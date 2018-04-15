#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>
#include <qDebug>

#include <opencv2/opencv.hpp>
#include "dir.h"

#define SRC "src"
#define DATA "data"
#define NEWUSER "s41"
#define IMG_FORMAT "jpg"
#define FACE_WIDTH 92
#define FACE_HEIGHT 112
#define LBP_FF "lbpcascade_frontalface.xml"

using namespace cv;

class ImageProcessing
{
public:
    ImageProcessing();
    Mat QImage2cvMat(QImage image);
    int ImageCutAndSave(QImage qimg, int id);

private:
    int faceWidth;
    int faceHeight;
    const std::string lbpcascade_frontalface;
};

#endif // IMAGEPROCESSING_H
