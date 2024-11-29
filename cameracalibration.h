#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>
#include <vector>
#include <QMouseEvent>

class CameraCalibration
{
public:
    CameraCalibration();
    ~CameraCalibration();
    cv::Mat getProcessedImage();

    bool processImage(const QImage &inputImage, std::vector<cv::Point2f> &corners);
<<<<<<< HEAD
    std::vector<cv::Point2f> getFeaturePoints(const QPixmap& pixmap);
    void setFeaturePoints(const QPixmap& pixmap, const std::vector<cv::Point2f>& corners);
    size_t customPixmapHash(const QPixmap& pixmap);
    // 正确的定义方式
    QHash<size_t, std::vector<cv::Point2f>> imageFeaturePoints;

=======
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82
    // void mousePressEvent(QMouseEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
    // void mouseReleaseEvent(QMouseEvent *event);

    static QImage MatToQImage(const cv::Mat &mat);

    static cv::Mat QImageToMat(const QImage &image);

private:
    cv::Mat processedMat;  // The processed image with corners drawn
<<<<<<< HEAD
    std::vector<std::vector<cv::Point2f>> allFeaturePoints; // 保存所有图片的特征点
=======
    // int selectedPointIndex;
    // std::vector<cv::Point2f> corners;
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82

    bool detectCorners(const cv::Mat &image, std::vector<cv::Point2f> &corners);

    void optimizeCorners(cv::Mat &image, std::vector<cv::Point2f> &corners);
};

#endif // CAMERACALIBRATION_H
