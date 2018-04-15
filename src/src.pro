QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceLogin
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    train.cpp \
    recognize.cpp \
    facelogin.cpp \
    entry.cpp \
    dir.cpp \
    imageprocessing.cpp

HEADERS += \
    train.h \
    recognize.h \
    facelogin.h \
    entry.h \
    dir.h \
    imageprocessing.h

FORMS += \
    facelogin.ui

# 指定生成路径
DESTDIR = $$PWD/../release

win32 {
INCLUDEPATH += E:\OpenCV_3.3.1\opencv-build\install\include
LIBS += -LE:\OpenCV_3.3.1\opencv-build\install\x86\mingw\bin \
    -llibopencv_core331 \
    -llibopencv_highgui331 \
    -llibopencv_imgproc331 #\
#    -llibopencv_imgcodecs331 \
#    -llibopencv_features2d331 \
#    -llibopencv_calib3d331
}

macx {
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.13.2/include
LIBS += -L/usr/local/Cellar/opencv/2.4.13.2/lib \
#    -lopencv_calib3d \
#    -lopencv_contrib \
    -lopencv_core \
    -lopencv_features2d \
#    -lopencv_flann \
#    -lopencv_gpu \
    -lopencv_highgui \
    -lopencv_imgproc \
#    -lopencv_legacy \
    -lopencv_ml \
#    -lopencv_nonfree \
    -lopencv_objdetect \
#    -lopencv_ocl \
#    -lopencv_photo \
#    -lopencv_stitching \
#    -lopencv_superres \
#    -lopencv_ts \
#    -lopencv_video \
#    -lopencv_videostab
}

unix:!macx{
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc #\
#    -lopencv_imgcodecs
}
