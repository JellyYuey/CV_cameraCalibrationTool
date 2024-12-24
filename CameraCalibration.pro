QT       += core gui
QT       += core gui widgets openglwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

Target = CameraCalibration

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameracalibration.cpp \
    cameracalibrationview.cpp \
    main.cpp \
    mainwindow.cpp \
    mylabel.cpp \
    settings.cpp \
    worldcoordinatecalculator.cpp

HEADERS += \
    cameracalibration.h \
    cameracalibrationview.h \
    mainwindow.h \
    mylabel.h \
    settings.h \
    worldcoordinatecalculator.h

FORMS += \
    mainwindow.ui \
    settings.ui

INCLUDEPATH += "C:/opencv/build/include"

LIBS += -L"C:/opencv/build/x64/vc16/lib"

# 示例：链接 opencv_world410.lib
LIBS += -lopencv_world4100d


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
