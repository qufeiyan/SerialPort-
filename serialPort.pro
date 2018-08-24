#-------------------------------------------------
#
# Project created by QtCreator 2018-07-12T23:59:13
#
#-------------------------------------------------

QT       += core gui serialport charts

INCLUDEPATH += C:\Users\Administrator\Desktop\serialPort\eigen

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialPort
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    network/SerialConnection.cpp \
    models/serialFrameModel.cpp \
    models/serialSettingModel.cpp \
    views/CustomDispView.cpp \
    views/TextDispView.cpp \
    views/ConnectionWidget.cpp \
    views/RealTimeChart.cpp \
    processing/Processing.cpp \
    processing/ElmProcessing.cpp \
    views/AHRSRealTimeChart.cpp \
    processing/AbstractAHRS.cpp \
    processing/MahonyAHRS.cpp \
    processing/MadgwickAHRS.cpp \
    processing/AHRSContext.cpp \
    sensorModel/geometryengine.cpp \
    sensorModel/mainwidget.cpp

HEADERS += \
        mainwindow.h \
    network/SerialConnection.h \
    models/serialFrameModel.h \
    models/serialSettingModel.h \
    views/CustomDispView.h \
    views/TextDispView.h \
    views/ConnectionWidget.h \
    views/RealTimeChart.h \
    processing/Processing.h \
    processing/ElmProcessing.h \
    views/AHRSRealTimeChart.h \
    processing/AbstractAHRS.h \
    processing/MahonyAHRS.h \
    processing/MadgwickAHRS.h \
    processing/AHRSContext.h \
    sensorModel/geometryengine.h \
    sensorModel/mainwidget.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    images.qrc \
    sensorModel/textures.qrc \
    sensorModel/shaders.qrc \
    data/data.qrc
