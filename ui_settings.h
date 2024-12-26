/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QLabel *label;
    QLabel *label_2;
    QComboBox *CalibrationBoardSize;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QComboBox *CalibrationBoardType;
    QComboBox *CameraModel;
    QLabel *labelRows;
    QSpinBox *BoardRows;
    QLabel *labelCols;
    QSpinBox *BoardCols;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName("Settings");
        Settings->resize(566, 400);
        label = new QLabel(Settings);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 100, 150, 22));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221")});
        font.setPointSize(12);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(Settings);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(160, 170, 100, 20));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        CalibrationBoardSize = new QComboBox(Settings);
        CalibrationBoardSize->addItem(QString());
        CalibrationBoardSize->addItem(QString());
        CalibrationBoardSize->addItem(QString());
        CalibrationBoardSize->addItem(QString());
        CalibrationBoardSize->setObjectName("CalibrationBoardSize");
        CalibrationBoardSize->setGeometry(QRect(330, 170, 71, 21));
        CalibrationBoardSize->setFont(font);
        label_3 = new QLabel(Settings);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(124, 240, 121, 20));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_4 = new QLabel(Settings);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(220, 10, 121, 41));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221")});
        font1.setPointSize(20);
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        okButton = new QPushButton(Settings);
        okButton->setObjectName("okButton");
        okButton->setGeometry(QRect(160, 330, 75, 24));
        okButton->setFont(font);
        cancelButton = new QPushButton(Settings);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setGeometry(QRect(310, 330, 75, 24));
        cancelButton->setFont(font);
        CalibrationBoardType = new QComboBox(Settings);
        CalibrationBoardType->addItem(QString());
        CalibrationBoardType->setObjectName("CalibrationBoardType");
        CalibrationBoardType->setGeometry(QRect(322, 100, 121, 27));
        CalibrationBoardType->setFont(font);
        CameraModel = new QComboBox(Settings);
        CameraModel->addItem(QString());
        CameraModel->addItem(QString());
        CameraModel->setObjectName("CameraModel");
        CameraModel->setGeometry(QRect(320, 240, 131, 21));
        CameraModel->setFont(font);
        labelRows = new QLabel(Settings);
        labelRows->setObjectName("labelRows");
        labelRows->setGeometry(QRect(124, 270, 121, 20));
        labelRows->setFont(font);
        labelRows->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BoardRows = new QSpinBox(Settings);
        BoardRows->setObjectName("BoardRows");
        BoardRows->setGeometry(QRect(320, 270, 71, 21));
        BoardRows->setFont(font);
        BoardRows->setMinimum(2);
        BoardRows->setMaximum(50);
        BoardRows->setValue(6);
        labelCols = new QLabel(Settings);
        labelCols->setObjectName("labelCols");
        labelCols->setGeometry(QRect(124, 300, 121, 20));
        labelCols->setFont(font);
        labelCols->setAlignment(Qt::AlignmentFlag::AlignCenter);
        BoardCols = new QSpinBox(Settings);
        BoardCols->setObjectName("BoardCols");
        BoardCols->setGeometry(QRect(320, 300, 71, 21));
        BoardCols->setFont(font);
        BoardCols->setMinimum(2);
        BoardCols->setMaximum(50);
        BoardCols->setValue(9);

        retranslateUi(Settings);

        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QCoreApplication::translate("Settings", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Settings", "Pattern Selection", nullptr));
        label_2->setText(QCoreApplication::translate("Settings", "Boxsize", nullptr));
        CalibrationBoardSize->setItemText(0, QCoreApplication::translate("Settings", "15", nullptr));
        CalibrationBoardSize->setItemText(1, QCoreApplication::translate("Settings", "20", nullptr));
        CalibrationBoardSize->setItemText(2, QCoreApplication::translate("Settings", "25", nullptr));
        CalibrationBoardSize->setItemText(3, QCoreApplication::translate("Settings", "30", nullptr));

        label_3->setText(QCoreApplication::translate("Settings", "Camera Model", nullptr));
        label_4->setText(QCoreApplication::translate("Settings", "Settings", nullptr));
        okButton->setText(QCoreApplication::translate("Settings", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("Settings", "Cancel", nullptr));
        CalibrationBoardType->setItemText(0, QCoreApplication::translate("Settings", "CheckerBox", nullptr));

        CameraModel->setItemText(0, QCoreApplication::translate("Settings", "Standard", nullptr));
        CameraModel->setItemText(1, QCoreApplication::translate("Settings", "Fisheye", nullptr));

        labelRows->setText(QCoreApplication::translate("Settings", "Board Rows", nullptr));
        labelCols->setText(QCoreApplication::translate("Settings", "Board Cols", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
