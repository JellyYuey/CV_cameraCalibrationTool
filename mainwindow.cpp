#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mylabel.h"
#include "cameracalibration.h"
#include "worldcoordinatecalculator.h"


#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QMenu>
#include <QDir>
#include <QPixmap>
#include <QImage>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,settingsDialog (new Settings(this))
{
    ui->setupUi(this);
    // ui->actionExport_Camera_Parameters->setEnabled(false);
    // ui->actionCalilbrate->setEnabled(false);
    // ui->actionSettings->setEnabled(false);

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

    int rows = 6;  // Number of rows in the chessboard pattern
    int cols = 9;  // Number of columns in the chessboard pattern
    float boxSize = settingsDialog->getBoardSize();  // Physical size of each grid, in millimeters


    calculator.setRows(rows);
    calculator.setCols(cols);
    calculator.setBoxSize(boxSize);


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

        // Calculate world coordinates
        std::vector<cv::Point3f> worldCorners = calculator.generateWorldCoordinates(corners);

        // Convert the processed Mat (with drawn corners) to QImage
        cv::Mat processedMat = calibration.getProcessedImage();  // Get the image with drawn corners

        QImage processedImage = calibration.MatToQImage(processedMat);  // Convert to QImage

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


/**
 * @brief MainWindow::on_actionAdd_Images_triggered
 * add images
 */
void MainWindow::on_actionAdd_Images_triggered()
{
    // 创建一个 QMenu 对象，作为下拉菜单
    QMenu *menu = new QMenu(this);

    // 添加两个选项到下拉菜单
    QAction *imageAction = new QAction("Image File", this);
    QAction *videoAction = new QAction("Video File", this);

    // 将选项添加到菜单
    menu->addAction(imageAction);
    menu->addAction(videoAction);

    // 连接信号到槽函数
    connect(imageAction, &QAction::triggered, this, &MainWindow::loadImageFiles);
    connect(videoAction, &QAction::triggered, this, &MainWindow::loadVideoFiles);

    // 将菜单绑定到按钮
    menu->exec(QCursor::pos()); // 在鼠标位置显示菜单
}

void MainWindow::loadImageFiles()
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

void MainWindow::loadVideoFiles()
{
    QStringList selectedFiles = QFileDialog::getOpenFileNames(nullptr, "Open Video Files", QDir::homePath(), "Videos (*.mp4 *.avi *.mkv)");

    foreach (const QString &selectedFile, selectedFiles) {
        qDebug() << "Selected video file:" << selectedFile;
        // 在这里添加加载或处理视频的逻辑
        processVideoFile(selectedFile); // 假设你有一个处理视频的方法
    }
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
void MainWindow::processVideoFile(const QString &selectedFile)
{
    cv::VideoCapture cap(selectedFile.toStdString());
    if (!cap.isOpened()) {
        qDebug() << "Error opening video stream or file";
        return;
    }

    int frameSkip = 30; // 每隔30帧抓取一次
    int frameCount = 0;

    cv::Mat frame;

    while (cap.read(frame)) {
        if (++frameCount % frameSkip == 0) {
            // 将 OpenCV 的 Mat 转换为 QImage
            cv::Mat rgbFrame;
            cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB); // 确保颜色通道顺序正确

            QImage qImg((const unsigned char*)(rgbFrame.data),
                        rgbFrame.cols, rgbFrame.rows,
                        rgbFrame.step, QImage::Format_RGB888);
            qImg = qImg.copy(); // 创建深拷贝以避免数据被释放的问题

            // 将 QImage 转换为 QPixmap 并添加到 UI 中
            QPixmap pixmap = QPixmap::fromImage(qImg);
            pixmap = pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            addPixmap(pixmap);

            qDebug() << "Captured frame at position" << cap.get(cv::CAP_PROP_POS_FRAMES);
        }
    }

}


