#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mylabel.h"
#include "CameraCalibration.h"
<<<<<<< HEAD

=======
#include "worldcoordinatecalculator.h"
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82


#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,settingsDialog (new Settings(this))
{
    ui->setupUi(this);
    // ui->actionExport_Camera_Parameters->setEnabled(false);
    // ui->actionCalilbrate->setEnabled(false);
    // ui->actionSettings->setEnabled(false);
<<<<<<< HEAD
    // 连接右侧图片区域的鼠标点击信号
    // connect(ui->label, &MyLabel::sig_click, this, &MainWindow::on_label_click);
=======
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_actionAdd_Images_triggered
 * Add images
 */
void MainWindow::addPixmap(const QPixmap &img)
{
<<<<<<< HEAD

=======
    // Create CameraCalibration object
    CameraCalibration calibration;
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82
    int rows = 6;  // Number of rows in the chessboard pattern
    int cols = 9;  // Number of columns in the chessboard pattern
    float boxSize = settingsDialog->getBoardSize();  // Physical size of each grid, in millimeters

    // Create WorldCoordinateCalculator object
<<<<<<< HEAD
    calculator.setRows(rows);
    calculator.setCols(cols);
    calculator.setBoxSize(boxSize);
=======
    WorldCoordinateCalculator calculator(rows, cols, boxSize);
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82

    // Convert QPixmap to QImage
    QImage qImage = img.toImage();

    // Check if QImage is null
    if (qImage.isNull()) {
        qDebug() << "QImage is null!";
        return;
    }

    // Call the image processing method
    std::vector<cv::Point2f> corners;
    bool found = calibration.processImage(qImage, corners);

    // If corners are detected, display the results
    if (found) {
<<<<<<< HEAD
        featurePoints=corners;
        // Calculate world coordinates
        std::vector<cv::Point3f> worldCorners = calculator.generateWorldCoordinates(corners);

=======
        // Calculate world coordinates
        std::vector<cv::Point3f> worldCorners = calculator.generateWorldCoordinates(corners);

        // Optionally, print the calculated world coordinates
        // std::cout << "World Coordinates: " << std::endl;
        // for (const auto& pt : worldCorners) {
        //     std::cout << pt.x << ", " << pt.y << ", " << pt.z << std::endl;
        // }
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82

        // Convert the processed Mat (with drawn corners) to QImage
        cv::Mat processedMat = calibration.getProcessedImage();  // Get the image with drawn corners

        QImage processedImage = calibration.MatToQImage(processedMat);  // Convert to QImage
<<<<<<< HEAD

=======
        QString filePath = "C:/Users/21064/Desktop/image.png";
        processedImage.save(filePath);  // Save the processed image
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82
        QPixmap processedPixmap = QPixmap::fromImage(processedImage);  // Convert to QPixmap

        // Display the processed image
        MyLabel *m = new MyLabel(processedPixmap, ui->listWidget);
        connect(m, &MyLabel::sig_switch_img, this, [this](const QPixmap& img){
            ui->label->setPixmap(img);
        });

        // Add the image to QListWidget
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(m->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, m);
        qDebug() << "Image added to QListWidget";
    } else {
        qDebug() << "Corners not found in image";
    }
}
<<<<<<< HEAD
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPixmap pixmap = ui->label->pixmap();
    auto hashValue=calibration.customPixmapHash(pixmap);
    // 打印哈希值
    qDebug() << "匹配图像Pixmap Hash: " << hashValue;
    // 获取当前图像的特征点
    std::vector<cv::Point2f> featurePoints = calibration.getFeaturePoints(pixmap);


    if (event->button() == Qt::LeftButton) {
        // 获取相对于 ui->label 的坐标
        QPoint labelPos = ui->label->mapFromGlobal(event->globalPos());


        // 计算缩放比例
        qreal scaleX = (qreal)ui->label->width() / (qreal)pixmap.width();
        qreal scaleY = (qreal)ui->label->height() / (qreal)pixmap.height();

        // 将点击位置从 label 坐标系转换为图像的像素坐标
        QPoint imagePos = QPoint(labelPos.x() / scaleX, labelPos.y() / scaleY);

        // 设置一个容忍度范围 (根据需要调整此值)
        const int tolerance = 10;  // 10 pixels tolerance

        // 遍历特征点，判断点击位置是否在特征点附近
        for (const auto& pt : featurePoints) {
            // 将特征点从 cv::Point2f 转换为 QPoint
            QPoint featurePoint(pt.x, pt.y);

            // 判断点击位置是否与特征点位置接近
            if ((imagePos - featurePoint).manhattanLength() <= tolerance) {
                qDebug() << "Mouse clicked near a feature point at: " << featurePoint;
                // 处理点击事件
                // 例如，你可以标记该特征点，或者执行其他操作
                break;  // 如果找到一个特征点就退出
            }
        }
    }else {
        qDebug() << "No feature points found for this image.";
    }
}

=======
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82



/**
 * @brief MainWindow::on_actionAdd_Images_triggered
 * add images
 */
void MainWindow::on_actionAdd_Images_triggered()
{
    QStringList selectedFiles = QFileDialog::getOpenFileNames(nullptr,"Open Files", QDir::homePath());

    foreach (const QString &selectedFile, selectedFiles) {
        qDebug() << selectedFile;
        QPixmap pixmap(selectedFile);
        pixmap = pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        addPixmap(pixmap);
    }
    ui->actionCalilbrate->setEnabled(true);
    ui->actionSettings->setEnabled(true);
}

/**
 * @brief MainWindow::on_actionSettings_triggered
 * open setting page
 */
void MainWindow::on_actionSettings_triggered()
{
    if (!settingsDialog) {
        settingsDialog = new Settings(this);
    }
    settingsDialog->exec();
}


void MainWindow::on_actionCalilbrate_triggered()
{
    //Clibration start
}

<<<<<<< HEAD
// // 鼠标点击事件槽函数
// void MainWindow::on_label_click(QMouseEvent *event)
// {
//     if (event->button() == Qt::LeftButton) {
//         // 获取右侧 label 中的鼠标点击位置
//         QPoint clickPos = event->pos();
//         qDebug() << "Mouse clicked at (MainWindow): " << clickPos;

//         // 在这里执行你想要的操作，例如标记交叉点，更新图像等
//     }
// }

=======
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82

