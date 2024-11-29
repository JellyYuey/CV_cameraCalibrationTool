#include "imageprocessor.h"
#include <QDebug>
#include <QImageReader>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

ImageProcessor::ImageProcessor()
{
    // 初始化
}

ImageProcessor::~ImageProcessor()
{
    // 释放资源（如果需要）
}

QPixmap ImageProcessor::preprocess(const QString &filePath, const QSize &targetSize)
{
    try {
        QImageReader reader(filePath);
        QImage image = reader.read();
        if (image.isNull()) {
            qWarning() << "Failed to load image:" << filePath;
            return QPixmap();
        }

        // 转换为灰度图像
        image = convertToGrayscale(image);
        if (image.format() != QImage::Format_Grayscale8) {
            qWarning() << "Image is not in Grayscale8 format.";
        }

        // 去噪
        image = reduceNoise(image);
        if (image.isNull()) {
            qWarning() << "Failed to reduce noise.";
            return QPixmap();
        }

        // 缩放图像
        image = image.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        if (image.isNull()) {
            qWarning() << "Failed to scale image.";
            return QPixmap();
        }

        // 使用棋盘格角点检测
        QPixmap result = detectAndDrawChessboardCorners(image);
        return result;
    }
    catch (const cv::Exception& e) {
        qWarning() << "OpenCV Exception in preprocess:" << e.what();
        return QPixmap();
    }
    catch (const std::exception& e) {
        qWarning() << "Standard Exception in preprocess:" << e.what();
        return QPixmap();
    }
    catch (...) {
        qWarning() << "Unknown Exception in preprocess";
        return QPixmap();
    }
}

QImage ImageProcessor::convertToGrayscale(const QImage &image)
{
    return image.convertToFormat(QImage::Format_Grayscale8);
}

QImage ImageProcessor::reduceNoise(const QImage &image)
{
    try {
        cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
        cv::Mat denoisedMat;
        cv::GaussianBlur(mat, denoisedMat, cv::Size(3, 3), 0);
        QImage denoisedImage(denoisedMat.data, denoisedMat.cols, denoisedMat.rows, denoisedMat.step, QImage::Format_Grayscale8);
        return denoisedImage.copy();  // 确保 QImage 拥有自己的数据
    }
    catch (const cv::Exception& e) {
        qWarning() << "OpenCV Exception in reduceNoise:" << e.what();
        return QImage();
    }
    catch (const std::exception& e) {
        qWarning() << "Standard Exception in reduceNoise:" << e.what();
        return QImage();
    }
    catch (...) {
        qWarning() << "Unknown Exception in reduceNoise";
        return QImage();
    }
}

QPixmap ImageProcessor::detectAndDrawChessboardCorners(const QImage &image)
{
    try {
        // 将 QImage 转换为 OpenCV Mat
        cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();

        // 设置棋盘格的行列数
        cv::Size boardSize(9, 6);  // 假设棋盘格的行列数是 9x6
        std::vector<cv::Point2f> corners;

        // 查找棋盘格角点
        bool found = cv::findChessboardCorners(mat, boardSize, corners);

        if (found) {
            // 精细化角点位置
            cv::cornerSubPix(mat, corners, cv::Size(11, 11), cv::Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 0.1));

            // 绘制角点
            cv::drawChessboardCorners(mat, boardSize, corners, found);

            // 转换回 QImage
            QImage resultImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
            return QPixmap::fromImage(resultImage);
        } else {
            qWarning() << "Chessboard corners not detected.";
            return QPixmap(); // 没有检测到角点时，返回空 QPixmap
        }
    }
    catch (const cv::Exception& e) {
        qWarning() << "OpenCV Exception in detectAndDrawChessboardCorners:" << e.what();
        return QPixmap();
    }
    catch (const std::exception& e) {
        qWarning() << "Standard Exception in detectAndDrawChessboardCorners:" << e.what();
        return QPixmap();
    }
    catch (...) {
        qWarning() << "Unknown Exception in detectAndDrawChessboardCorners";
        return QPixmap();
    }
}
