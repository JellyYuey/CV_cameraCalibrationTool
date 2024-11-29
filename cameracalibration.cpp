#include "CameraCalibration.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>  // Include OpenCV drawing module
#include <QFileDialog>
#include <QMouseEvent>
#include <QHash>

CameraCalibration::CameraCalibration() {}
CameraCalibration::~CameraCalibration() {}

cv::Mat CameraCalibration::getProcessedImage()
{
    return processedMat;  // Return the processed image
}

/**
 * @brief Process the input image to find chessboard corners and draw them.
 *
 * This function converts the input image to grayscale, detects the chessboard corners,
 * refines their locations, and draws them on the image.
 *
 * @param image The input image in QImage format.
 * @param corners A reference to a vector of 2D points that will contain the detected corners.
 * @return true if corners were found and processed, false otherwise.
 */
bool CameraCalibration::processImage(const QImage &image, std::vector<cv::Point2f> &corners)
{
    // Convert QImage to cv::Mat
    cv::Mat matImage = QImageToMat(image);

    if (matImage.empty()) {
        qDebug() << "Empty image!";
        return false;
    }

    // Convert image to grayscale
    cv::Mat grayImage;
    cv::cvtColor(matImage, grayImage, cv::COLOR_BGR2GRAY);

    // Detect corners
    bool found = cv::findChessboardCorners(grayImage, cv::Size(9, 6), corners,
                                           cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

    if (found) {
        // 将图像和特征点保存到 imageFeaturePoints 中
        // 假设 image 是一个 QImage 对象
        QPixmap* pixmap = new QPixmap(QPixmap::fromImage(image)); // 创建 QPixmap 对象的指针

        setFeaturePoints(*pixmap,corners);
        allFeaturePoints.push_back(corners);
        // Refine corner positions
        cv::cornerSubPix(grayImage, corners, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));

        // Draw the detected corners on the image
        cv::drawChessboardCorners(matImage, cv::Size(9, 6), corners, found);
        processedMat = matImage.clone();  // Save the processed image
    } else {
        qDebug() << "Corners not found!";
    }

    // Return the modified image (with drawn corners)
    return found;
}
size_t CameraCalibration::customPixmapHash(const QPixmap& pixmap) {
    QImage image = pixmap.toImage();  // 将 QPixmap 转换为 QImage
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << image;  // 将 QImage 写入流
    return qHash(byteArray);  // 使用 byteArray 的哈希值
}

std::vector<cv::Point2f> CameraCalibration::getFeaturePoints(const QPixmap& pixmap) {
    size_t pixmapHash = customPixmapHash(pixmap);  // 使用 pixmap 的哈希值作为键
    auto it = imageFeaturePoints.find(pixmapHash); // 查找对应的哈希值
    if (it != imageFeaturePoints.end()) {
        return it.value(); // 使用 it.value() 获取特征点列表
    }
    return std::vector<cv::Point2f>(); // 如果没有找到，返回空 vector
}
// 存储特征点的函数
void CameraCalibration::setFeaturePoints(const QPixmap& pixmap, const std::vector<cv::Point2f>& corners) {
    size_t pixmapHash = customPixmapHash(pixmap);  // 使用 pixmap 的哈希值作为键
    imageFeaturePoints[pixmapHash] = corners;      // 存储特征点
}


/**
 * @brief Detect chessboard corners in the image.
 *
 * This function detects the chessboard corners without any additional processing like drawing.
 *
 * @param image The input image in cv::Mat format.
 * @param corners A reference to a vector of 2D points that will contain the detected corners.
 * @return true if corners were found, false otherwise.
 */
bool CameraCalibration::detectCorners(const cv::Mat &image, std::vector<cv::Point2f> &corners)
{
    // Detect chessboard corners
    return cv::findChessboardCorners(image, cv::Size(9, 6), corners);
}

/**
 * @brief Optimize the corner locations.
 *
 * This function refines the detected corner locations using cornerSubPix.
 *
 * @param image The input image in cv::Mat format.
 * @param corners A reference to the vector of detected corners to be refined.
 */
void CameraCalibration::optimizeCorners(cv::Mat &image, std::vector<cv::Point2f> &corners)
{
    // Refine corner positions
    cv::cornerSubPix(image, corners, cv::Size(11, 11), cv::Size(-1, -1),
                     cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
}

/**
 * @brief Convert QImage to cv::Mat.
 *
 * This function converts a QImage to a cv::Mat, handling different image formats.
 *
 * @param image The input image in QImage format.
 * @return The converted cv::Mat.
 */
cv::Mat CameraCalibration::QImageToMat(const QImage &image)
{
    if (image.isNull()) {
        qDebug() << "QImage is null!";
        return cv::Mat();
    }

    // Use if-else instead of switch statement
    if (image.format() == QImage::Format_RGB888) {
        return cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
    }
    else if (image.format() == QImage::Format_Indexed8) {
        return cv::Mat(image.height(), image.width(), CV_8U, (void*)image.bits(), image.bytesPerLine());
    }
    else if (image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_RGB32) {
        // Handle ARGB32 and RGB32 formats by removing the alpha channel
        cv::Mat temp(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
        cv::Mat rgbMat;
        cv::cvtColor(temp, rgbMat, cv::COLOR_RGBA2RGB); // Convert RGBA to RGB
        return rgbMat;
    }
    else {
        qDebug() << "Unsupported image format";
        return cv::Mat();
    }
}

/**
 * @brief Convert cv::Mat to QImage.
 *
 * This function converts a cv::Mat to a QImage, handling different formats (grayscale, RGB, RGBA).
 *
 * @param mat The input image in cv::Mat format.
 * @return The converted QImage.
 */
QImage CameraCalibration::MatToQImage(const cv::Mat &mat)
{
    if (mat.empty()) {
        qDebug() << "Empty Mat!";
        return QImage();
    }

    qDebug() << mat.channels();
    if (mat.channels() == 1) {
        // If the image is grayscale
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return img.copy();
    } else if (mat.channels() == 3) {
        // If the image is colored (BGR format)
        cv::Mat rgbMat;
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);  // Convert BGR to RGB
        QImage img(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888);
        return img.copy();
    } else if (mat.channels() == 4) {
        // If the image has an alpha channel
        cv::Mat rgbaMat;
        cv::cvtColor(mat, rgbaMat, cv::COLOR_BGRA2RGBA);  // Convert BGRA to RGBA
        QImage img(rgbaMat.data, rgbaMat.cols, rgbaMat.rows, rgbaMat.step, QImage::Format_RGBA8888);
        return img.copy();
    } else {
        qDebug() << "Unsupported image format";
        return QImage();
    }
}
