// cameracalibration.cpp
#include "cameracalibration.h"
#include <QDebug>
#include <iomanip>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// 构造函数
CameraCalibration::CameraCalibration() : totalAvgErr(0.0) {
  // 初始化相机矩阵为单位矩阵
  cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
  // 初始化畸变系数为零向量（标准模型通常使用5个系数）
  distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
}

// 析构函数
CameraCalibration::~CameraCalibration() {}

// 计算重投影误差
double CameraCalibration::computeReprojectionErrors(
    const std::vector<std::vector<cv::Point2f>> &imagePoints,
    const std::vector<std::vector<cv::Point3f>> &objectPoints,
    const QString &cameraType) {

  std::vector<cv::Point2f> projectedPoints;
  double totalErr = 0.0;
  int totalPoints = 0;

  for (size_t i = 0; i < objectPoints.size(); ++i) {
    if (cameraType.toLower() == "fisheye") {
      cv::fisheye::projectPoints(objectPoints[i], projectedPoints, rvecs[i],
                                 tvecs[i], cameraMatrix, distCoeffs);
    } else {
      cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix,
                        distCoeffs, projectedPoints);
    }

    double err = cv::norm(imagePoints[i], projectedPoints, cv::NORM_L2);
    size_t n = objectPoints[i].size();
    double mean_error = std::sqrt(err * err / n);

    totalErr += err * err;
    totalPoints += n;
  }

  return std::sqrt(totalErr / totalPoints);
}

// 标定函数
bool CameraCalibration::calibrate(
    const std::vector<std::vector<cv::Point2f>> &inputImagePoints,
    const std::vector<std::vector<cv::Point3f>> &inputObjectPoints,
    const QString &cameraType, const cv::Size &imageSize) {

  try {
    // 1. 输入数据的基本检查
    if (inputImagePoints.empty() || inputObjectPoints.empty()) {
      qWarning() << "No points available for calibration";
      return false;
    }

    if (inputImagePoints.size() != inputObjectPoints.size()) {
      qWarning() << "Number of images doesn't match between image points and "
                    "object points";
      return false;
    }

    // 2. 确保有足够的图像进行标定
    if (inputImagePoints.size() < 5) { // 增加最小图像数量，建议至少5张
      qWarning() << "Need at least 5 images for calibration";
      return false;
    }

    // 3. 创建本地可修改的拷贝
    std::vector<std::vector<cv::Point2f>> imgPointsCopy = inputImagePoints;
    std::vector<std::vector<cv::Point3f>> objPointsCopy = inputObjectPoints;

    // 4. 标定过程封装为一个lambda函数
    auto performCalibration = [&](double &rms) -> bool {
      if (cameraType.toLower() == "standard") {
        // 标准相机标定
        int flags = 0;
        // 可以根据需要调整标志，例如固定某些畸变系数
        // flags |= cv::CALIB_FIX_K3; // 示例：固定K3

        // 设置终止条件
        cv::TermCriteria criteria(
            cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 1e-8);

        rms = cv::calibrateCamera(objPointsCopy, imgPointsCopy, imageSize,
                                  cameraMatrix, distCoeffs, rvecs, tvecs, flags,
                                  criteria);

        qDebug() << "Standard camera calibration RMS:" << rms;
      } else if (cameraType.toLower() == "fisheye") {
        // 鱼眼相机标定
        distCoeffs =
            cv::Mat::zeros(4, 1, CV_64F); // 鱼眼模型通常使用4个畸变系数

        int flags = cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC |
                    cv::fisheye::CALIB_CHECK_COND;
        // 不固定畸变参数，允许优化所有参数

        // 设置终止条件
        cv::TermCriteria criteria(
            cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 1e-8);

        rms = cv::fisheye::calibrate(objPointsCopy, imgPointsCopy, imageSize,
                                     cameraMatrix, distCoeffs, rvecs, tvecs,
                                     flags, criteria);

        qDebug() << "Fisheye camera calibration RMS:" << rms;
      } else {
        qWarning() << "Unsupported camera type:" << cameraType;
        return false;
      }

      return true;
    };

    // 5. 执行初次标定
    double rms = 0.0;
    if (!performCalibration(rms)) {
      return false;
    }

    // 6. 计算初始重投影误差
    totalAvgErr = computeReprojectionErrors(inputImagePoints, inputObjectPoints,
                                            cameraType);
    qDebug() << "Initial reprojection error:" << totalAvgErr;

    // 7. 标记高误差图像
    std::vector<size_t> badImages;
    double errorThreshold = 3.0; // 设置误差阈值，例如1.5像素

    for (size_t i = 0; i < objPointsCopy.size(); ++i) {
      std::vector<cv::Point2f> projectedPoints;
      if (cameraType.toLower() == "fisheye") {
        cv::fisheye::projectPoints(objPointsCopy[i], projectedPoints, rvecs[i],
                                   tvecs[i], cameraMatrix, distCoeffs);
      } else {
        cv::projectPoints(objPointsCopy[i], rvecs[i], tvecs[i], cameraMatrix,
                          distCoeffs, projectedPoints);
      }

      double err = cv::norm(imgPointsCopy[i], projectedPoints, cv::NORM_L2);
      size_t n = objPointsCopy[i].size();
      double mean_error = std::sqrt(err * err / n);

      if (mean_error > errorThreshold) {
        badImages.push_back(i);
      }
    }

    qDebug() << "Total reprojection error:" << totalAvgErr;

    // 8. 移除高误差的图像并重新标定
    if (!badImages.empty()) {
      qDebug() << "Removing" << badImages.size() << "images with high error.";

      // 从后往前移除，避免索引问题
      for (auto it = badImages.rbegin(); it != badImages.rend(); ++it) {
        const size_t idx = *it;
        imgPointsCopy.erase(imgPointsCopy.begin() + idx);
        objPointsCopy.erase(objPointsCopy.begin() + idx);
      }

      // 确保移除后仍有足够的图像
      if (imgPointsCopy.size() < 5) {
        qWarning()
            << "Not enough images left after removing high-error images.";
        return false;
      }

      // 重新标定
      double rms_after = 0.0;
      if (!performCalibration(rms_after)) {
        return false;
      }

      qDebug() << "Calibration RMS after removing bad images:" << rms_after;

      // 重新计算重投影误差
      totalAvgErr =
          computeReprojectionErrors(imgPointsCopy, objPointsCopy, cameraType);
      qDebug() << "Final reprojection error after recalibration:"
               << totalAvgErr;
    }

    // 9. 保存标定结果（可选，根据需求）
    // 例如，将相机矩阵和畸变系数保存到文件
    /*
    cv::FileStorage fs_out("calibration_result.yml", cv::FileStorage::WRITE);
    if (fs_out.isOpened()) {
        fs_out << "camera_matrix" << cameraMatrix;
        fs_out << "distortion_coefficients" << distCoeffs;
        fs_out.release();
        qDebug() << "Calibration parameters saved to calibration_result.yml";
    } else {
        qWarning() << "Failed to save calibration parameters to file.";
    }
    */

    // 10. 返回标定结果
    return true;

  } catch (const cv::Exception &e) {
    qWarning() << "OpenCV exception during calibration:" << e.what();
    return false;
  } catch (const std::exception &e) {
    qWarning() << "Standard exception during calibration:" << e.what();
    return false;
  } catch (...) {
    qWarning() << "Unknown exception during calibration";
    return false;
  }
}
