#-------------------------------------------------
#
# Project created by QtCreator 2014-12-28T01:05:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = findRectObjects
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    worker-thread.cpp

HEADERS  += mainwindow.h \
    worker-thread.h

FORMS    += mainwindow.ui

INCLUDEPATH += d:\\Source\\opencv\\build_mingw\\install\\include
INCLUDEPATH += d:\\Source\\opencv\\build_mingw\\install\\include\\opencv

LIBS += D:\\Source\\opencv\\build_mingw\\install\\x64\\mingw\\lib\\libopencv_core248d.dll.a
LIBS += D:\\Source\\opencv\\build_mingw\\install\\x64\\mingw\\lib\\libopencv_highgui248d.dll.a
LIBS += D:\\Source\\opencv\\build_mingw\\install\\x64\\mingw\\lib\\libopencv_imgproc248d.dll.a
LIBS += D:\\Source\\opencv\\build_mingw\\install\\x64\\mingw\\lib\\libopencv_features2d248d.dll.a
LIBS += D:\\Source\\opencv\\build_mingw\\install\\x64\\mingw\\lib\\libopencv_calib3d248d.dll.a

OTHER_FILES +=

RESOURCES += \
    findRectObjects.qrc
