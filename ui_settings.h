/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
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

QT_BEGIN_NAMESPACE

class Ui_Dialog
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

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(566, 342);
        label = new QLabel(Dialog);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 100, 130, 22));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221")});
        font.setPointSize(12);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(Dialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(160, 170, 81, 20));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        CalibrationBoardSize = new QComboBox(Dialog);
        CalibrationBoardSize->addItem(QString());
        CalibrationBoardSize->addItem(QString());
        CalibrationBoardSize->addItem(QString());
        CalibrationBoardSize->setObjectName("CalibrationBoardSize");
        CalibrationBoardSize->setGeometry(QRect(330, 170, 71, 21));
        CalibrationBoardSize->setFont(font);
        label_3 = new QLabel(Dialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(124, 240, 121, 20));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_4 = new QLabel(Dialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(220, 10, 121, 41));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221")});
        font1.setPointSize(20);
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        okButton = new QPushButton(Dialog);
        okButton->setObjectName("okButton");
        okButton->setGeometry(QRect(160, 300, 75, 24));
        okButton->setFont(font);
        cancelButton = new QPushButton(Dialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setGeometry(QRect(310, 300, 75, 24));
        cancelButton->setFont(font);
        CalibrationBoardType = new QComboBox(Dialog);
        CalibrationBoardType->addItem(QString());
        CalibrationBoardType->addItem(QString());
        CalibrationBoardType->setObjectName("CalibrationBoardType");
        CalibrationBoardType->setGeometry(QRect(322, 100, 121, 27));
        CalibrationBoardType->setFont(font);
        CameraModel = new QComboBox(Dialog);
        CameraModel->addItem(QString());
        CameraModel->addItem(QString());
        CameraModel->setObjectName("CameraModel");
        CameraModel->setGeometry(QRect(320, 240, 131, 21));
        CameraModel->setFont(font);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Pattern Selection", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Boxsize", nullptr));
        CalibrationBoardSize->setItemText(0, QCoreApplication::translate("Dialog", "20", nullptr));
        CalibrationBoardSize->setItemText(1, QCoreApplication::translate("Dialog", "25", nullptr));
        CalibrationBoardSize->setItemText(2, QCoreApplication::translate("Dialog", "30", nullptr));

        label_3->setText(QCoreApplication::translate("Dialog", "Camera Model", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "Settings", nullptr));
        okButton->setText(QCoreApplication::translate("Dialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("Dialog", "Cancel", nullptr));
        CalibrationBoardType->setItemText(0, QCoreApplication::translate("Dialog", "CheckerBox", nullptr));
        CalibrationBoardType->setItemText(1, QCoreApplication::translate("Dialog", "Triangle", nullptr));

        CameraModel->setItemText(0, QCoreApplication::translate("Dialog", "Standard", nullptr));
        CameraModel->setItemText(1, QCoreApplication::translate("Dialog", "Fisheye", nullptr));

    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
