// #include "cameracalibrationview.h"
// // #include <glm/glm.hpp>
// // #include <glm/gtc/matrix_transform.hpp>


// void CameraCalibrationView::initializeGL()
// {
//     glEnable(GL_DEPTH_TEST); // 启用深度测试
// }

// void CameraCalibrationView::resizeGL(int w, int h)
// {
//     glViewport(0, 0, w, h);
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     createPerspectiveMatrix(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
//     glMatrixMode(GL_MODELVIEW);
// }

// void CameraCalibrationView::paintGL()
// {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     glLoadIdentity();
//     // gluLookAt(0.0, 0.0, 10.0,  // 相机位置
//     //           0.0, 0.0, 0.0,   // 看向原点
//     //           0.0, 1.0, 0.0);  // 上方向

//     // 绘制坐标系
//     drawCoordinateAxes();

//     // 绘制标定板图像（假设已经计算出标定板在3D空间中的位置）
//     drawCalibrationPattern();
// }
// void CameraCalibrationView::drawCoordinateAxes()
// {
//     glBegin(GL_LINES);
//     // 绘制X轴（红色）
//     glColor3f(1.0f, 0.0f, 0.0f);
//     glVertex3f(0.0f, 0.0f, 0.0f);
//     glVertex3f(1.0f, 0.0f, 0.0f);

//     // 绘制Y轴（绿色）
//     glColor3f(0.0f, 1.0f, 0.0f);
//     glVertex3f(0.0f, 0.0f, 0.0f);
//     glVertex3f(0.0f, 1.0f, 0.0f);

//     // 绘制Z轴（蓝色）
//     glColor3f(0.0f, 0.0f, 1.0f);
//     glVertex3f(0.0f, 0.0f, 0.0f);
//     glVertex3f(0.0f, 0.0f, 1.0f);
//     glEnd();
// }
// void CameraCalibrationView::drawCalibrationPattern()
// {
//     glBegin(GL_QUADS);
//     glColor3f(0.8f, 0.8f, 0.8f);  // 灰色
//     glVertex3f(-1.0f, -1.0f, 0.0f);
//     glVertex3f( 1.0f, -1.0f, 0.0f);
//     glVertex3f( 1.0f,  1.0f, 0.0f);
//     glVertex3f(-1.0f,  1.0f, 0.0f);
//     glEnd();
// }
// QMatrix4x4 CameraCalibrationView::createPerspectiveMatrix(float fov, float aspect, float nearPlane, float farPlane) {
//     QMatrix4x4 perspective;
//     perspective.setToIdentity();
//     perspective.perspective(fov, aspect, nearPlane, farPlane);
//     return perspective;
// }
