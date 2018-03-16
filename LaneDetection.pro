#-------------------------------------------------
#
# Project created by QtCreator 2018-03-03T19:51:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaneDetection
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
        dialog.cpp \
    lanedetection.cpp

HEADERS += \
        dialog.h \
    lanedetection.h

FORMS += \
        dialog.ui


LIBS += -lopencv_core \
    -lopencv_videoio \
    #-lopencv_calib3d \
    #-lopencv_contrib \
    -lopencv_features2d \
    #-lopencv_flann \
    -lopencv_highgui \
    -lopencv_imgproc \
    #-lopencv_legacy \
    #-lopencv_ml \
    #-lopencv_objdetect \
    #-lopencv_photo \
    #-lopencv_videostab \
    #-lopencv_stitching \
    #-lopencv_ts \
    -lopencv_video
