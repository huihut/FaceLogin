#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>

#include <QMainWindow>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QMessageBox>
#include <QtWidgets>
#include <QDir>

#include <opencv2/opencv.hpp>

#include "recognize.h"
#include "train.h"
#include "dir.h"
#include "imageprocessing.h"

#define ENTRY_TIMES 10  // 录入人脸数据次数（录入时拍多少张照）

using namespace cv;
using namespace std;

namespace Ui {
class FaceLogin;
}

class FaceLogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit FaceLogin(QWidget *parent = 0);
    ~FaceLogin();

private slots:
    // Camera
    void startCamera();
    void stopCamera();
    void setCamera(const QCameraInfo &cameraInfo);
    void updateCameraState(QCamera::State state);
    void updateCameraDevice(QAction *action);
    void displayCameraError();

    // Login & Register
    void displayViewfinder();
    void displayCapturedImage();

    // Login
    void loginTakeImageButtonClick();
    void readyForLoginCapture(bool ready);
    void processLoginCapturedImage(int requestId, const QImage& img);
    void loginImageSaved(int id, const QString &fileName);
    void displayLoginCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString);
    void registerButtonClick();

    // Register
    void readyForRegisterCapture(bool ready);
    void processRegisterCapturedImage(int requestId, QString str);
    void registerTakeImageButtonClick();
    void registerImageSaved(int id, const QString &fileName);
    void displayRegisterCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString);
    void registerCancelButtonClick();

private:
    enum LoginState
    {
        loggingin,
        loginSuccess,
        loginFail,
        registering,
        registerSucess,
        registerFail
    } loginState;

    Ui::FaceLogin *ui;

    QCamera *camera;
    QCameraImageCapture *loginImageCapture, *registerImageCapture;
    QAction *videoDeviceAction;
    QActionGroup *videoDevicesGroup;
    QString registerUserName;
    bool isLoginCapturingImage, isRegisterCapturingImage;
    bool applicationExiting;
    const unsigned int entryTimes; // 录入人脸数据次数（录入时拍多少张照）
    unsigned int entryTimesNow;    // 当前录入人脸数据次数
};

#endif // LOGIN_H
