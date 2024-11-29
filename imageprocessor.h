#ifndef IMAGEPORCESSOR_H
#define IMAGEPORCESSOR_H

#include <QPixmap>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

class ImageProcessor
{
public:
    ImageProcessor();
    ~ImageProcessor();

    // 预处理图像的方法
    QPixmap preprocess(const QString &filePath, const QSize &targetSize);

    QPixmap detectAndDrawChessboardCorners(const QImage &image);

private:
    // 预处理相关的方法
    QImage convertToGrayscale(const QImage &image);
    QImage reduceNoise(const QImage &image);

    // OpenCV 特征检测器
    cv::Ptr<cv::Feature2D> featureDetector;
};

#endif // IMAGEPORCESSOR_H
