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
    // void mousePressEvent(QMouseEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
    // void mouseReleaseEvent(QMouseEvent *event);

    static QImage MatToQImage(const cv::Mat &mat);

    static cv::Mat QImageToMat(const QImage &image);

private:
    cv::Mat processedMat;  // The processed image with corners drawn
    // int selectedPointIndex;
    // std::vector<cv::Point2f> corners;

    bool detectCorners(const cv::Mat &image, std::vector<cv::Point2f> &corners);

    void optimizeCorners(cv::Mat &image, std::vector<cv::Point2f> &corners);
};

#endif // CAMERACALIBRATION_H
