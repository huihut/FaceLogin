QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceLogin
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        login.cpp

HEADERS += \
        login.h

FORMS += \
        login.ui

win32 {

INCLUDEPATH += E:\OpenCV_3.3.1\opencv-build\install\include

LIBS += -LE:\OpenCV_3.3.1\opencv-build\install\x86\mingw\bin \
    -llibopencv_core331 \
    -llibopencv_highgui331 \
    -llibopencv_imgcodecs331 \
    -llibopencv_imgproc331 \
    -llibopencv_features2d331 \
    -llibopencv_calib3d331

}

macx {

}

unix:!macx{

}
