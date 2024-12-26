#include "imageprocessor.h"
#include <QDebug>
#include <QImageReader>
#include <opencv2/calib3d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

ImageProcessor::ImageProcessor()
    : m_boardRows(6), m_boardCols(9), m_imagePoints() {}

ImageProcessor::~ImageProcessor() {}

void ImageProcessor::setBoardSize(int rows, int cols) {
  qDebug() << "In ImageProcessor, Setting board size:" << rows << "x" << cols;
  m_boardRows = rows;
  m_boardCols = cols;
}

std::vector<std::vector<cv::Point2f>> ImageProcessor::getImagePoints() const {
  return m_imagePoints;
}

void ImageProcessor::clearImagePoints() { m_imagePoints.clear(); }

QPixmap ImageProcessor::preprocess(const QImage &image,
                                   const QSize &targetSize) {
  if (image.isNull()) {
    qWarning() << "Input image is null";
    return QPixmap();
  }

  m_originalSize = image.size();
  return detectAndDrawChessboardCorners(image, true);
}

// 重命名原来的preprocess为preprocessFromFile
QPixmap ImageProcessor::preprocessFromFile(const QString &filePath,
                                           const QSize &targetSize) {
  QImageReader reader(filePath);
  QImage image = reader.read();
  return preprocess(image, targetSize);
}

bool ImageProcessor::processForCalibration(const QString &filePath) {
  try {
    QImageReader reader(filePath);
    QImage image = reader.read();
    if (image.isNull()) {
      qWarning() << "Failed to load image for calibration:" << filePath;
      return false;
    }

    // 转换为灰度图像并去噪，但不缩放
    image = convertToGrayscale(image);
    image = reduceNoise(image);

    // 在原始尺寸图像上检测角点，但不绘制
    QPixmap result = detectAndDrawChessboardCorners(image, false);
    return !result.isNull();
  } catch (const cv::Exception &e) {
    qWarning() << "OpenCV Exception in processForCalibration:" << e.what();
    return false;
  } catch (...) {
    qWarning() << "Unknown Exception in processForCalibration";
    return false;
  }
}

QImage ImageProcessor::convertToGrayscale(const QImage &image) {
  return image.convertToFormat(QImage::Format_Grayscale8);
}

QImage ImageProcessor::reduceNoise(const QImage &image) {
  try {
    cv::Mat mat =
        cv::Mat(image.height(), image.width(), CV_8UC1,
                const_cast<uchar *>(image.bits()), image.bytesPerLine())
            .clone();
    cv::Mat denoisedMat;
    cv::GaussianBlur(mat, denoisedMat, cv::Size(3, 3), 0);
    QImage denoisedImage(denoisedMat.data, denoisedMat.cols, denoisedMat.rows,
                         denoisedMat.step, QImage::Format_Grayscale8);
    return denoisedImage.copy();
  } catch (const cv::Exception &e) {
    qWarning() << "OpenCV Exception in reduceNoise:" << e.what();
    return QImage();
  } catch (...) {
    qWarning() << "Unknown Exception in reduceNoise";
    return QImage();
  }
}

QPixmap ImageProcessor::detectAndDrawChessboardCorners(const QImage &image,
                                                       bool drawCorners) {
  try {
    if (m_boardCols <= 2 || m_boardRows <= 2) {
      qWarning() << "Invalid board size";
      return QPixmap();
    }

    // QImage转cv::Mat
    cv::Mat matImage = QImageToMat(image);
    if (matImage.empty())
      return QPixmap();

    // 转灰度
    cv::Mat grayImage;
    cv::cvtColor(matImage, grayImage, cv::COLOR_BGR2GRAY);

    cv::Size boardSize(m_boardCols - 1, m_boardRows - 1);
    std::vector<cv::Point2f> corners;

    // 角点检测
    bool found = cv::findChessboardCorners(grayImage, boardSize, corners,
                                           cv::CALIB_CB_ADAPTIVE_THRESH |
                                               cv::CALIB_CB_NORMALIZE_IMAGE);

    if (found) {
      // 亚像素优化
      cv::cornerSubPix(
          grayImage, corners, cv::Size(11, 11), cv::Size(-1, -1),
          cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER,
                           30, 0.1));

      m_imagePoints.push_back(corners);

      if (drawCorners) {
        cv::drawChessboardCorners(matImage, boardSize, corners, found);
        QImage resultImage = MatToQImage(matImage);
        return QPixmap::fromImage(resultImage);
      }
    }
    return QPixmap();
  } catch (const cv::Exception &e) {
    qWarning() << "OpenCV error:" << e.what();
    return QPixmap();
  }
}

// QImage转cv::Mat
cv::Mat ImageProcessor::QImageToMat(const QImage &image) {
  if (image.isNull())
    return cv::Mat();

  if (image.format() == QImage::Format_RGB888) {
    return cv::Mat(image.height(), image.width(), CV_8UC3, (void *)image.bits(),
                   image.bytesPerLine());
  } else if (image.format() == QImage::Format_Indexed8) {
    return cv::Mat(image.height(), image.width(), CV_8U, (void *)image.bits(),
                   image.bytesPerLine());
  } else if (image.format() == QImage::Format_ARGB32 ||
             image.format() == QImage::Format_RGB32) {
    cv::Mat temp(image.height(), image.width(), CV_8UC4, (void *)image.bits(),
                 image.bytesPerLine());
    cv::Mat rgbMat;
    cv::cvtColor(temp, rgbMat, cv::COLOR_RGBA2RGB);
    return rgbMat;
  }
  return cv::Mat();
}

// cv::Mat转QImage
QImage ImageProcessor::MatToQImage(const cv::Mat &mat) {
  if (mat.empty())
    return QImage();

  if (mat.channels() == 1) {
    return QImage(mat.data, mat.cols, mat.rows, mat.step,
                  QImage::Format_Grayscale8)
        .copy();
  } else if (mat.channels() == 3) {
    cv::Mat rgbMat;
    cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
    return QImage(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step,
                  QImage::Format_RGB888)
        .copy();
  } else if (mat.channels() == 4) {
    cv::Mat rgbaMat;
    cv::cvtColor(mat, rgbaMat, cv::COLOR_BGRA2RGBA);
    return QImage(rgbaMat.data, rgbaMat.cols, rgbaMat.rows, rgbaMat.step,
                  QImage::Format_RGBA8888)
        .copy();
  }
  return QImage();
}

QSize ImageProcessor::getOriginalImageSize() const { return m_originalSize; }