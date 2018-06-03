#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>
#include <qDebug>
#include <opencv2/opencv.hpp>
#include "dir.h"

#define SRC "src"           // src 路径
#define DATASET "dataset"   // dataset 路径
#define MODEL "model"       // 模型路径
#define NEWUSER "s41"       // 新用户默认名
#define IMG_FORMAT "jpg"    // 注册存储的图片格式
#define FACE_WIDTH 92       // 数据集的宽
#define FACE_HEIGHT 112     // 数据集的高
#define LBP_FF "lbpcascade_frontalface.xml"     // 前置人脸分类器

using namespace cv;

class ImageProcessing
{
public:
    ImageProcessing();
    Mat QImage2cvMat(QImage image);
    Mat norm_0_255(InputArray _src);
    int ImageCutAndSave(QImage qimg, QString userName, int id, QString &imgPath);

private:
    int faceWidth;
    int faceHeight;
    const std::string lbpcascade_frontalface;
    QString newUserName;
};

#endif // IMAGEPROCESSING_H
