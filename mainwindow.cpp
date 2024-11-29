#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mylabel.h"
#include "CameraCalibration.h"
#include "worldcoordinatecalculator.h"


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
    // Create CameraCalibration object
    CameraCalibration calibration;
    int rows = 6;  // Number of rows in the chessboard pattern
    int cols = 9;  // Number of columns in the chessboard pattern
    float boxSize = settingsDialog->getBoardSize();  // Physical size of each grid, in millimeters

    // Create WorldCoordinateCalculator object
    WorldCoordinateCalculator calculator(rows, cols, boxSize);

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

        // Optionally, print the calculated world coordinates
        // std::cout << "World Coordinates: " << std::endl;
        // for (const auto& pt : worldCorners) {
        //     std::cout << pt.x << ", " << pt.y << ", " << pt.z << std::endl;
        // }

        // Convert the processed Mat (with drawn corners) to QImage
        cv::Mat processedMat = calibration.getProcessedImage();  // Get the image with drawn corners

        QImage processedImage = calibration.MatToQImage(processedMat);  // Convert to QImage
        QString filePath = "C:/Users/21064/Desktop/image.png";
        processedImage.save(filePath);  // Save the processed image
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


