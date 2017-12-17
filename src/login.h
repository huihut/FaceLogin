#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QCamera>
#include <QCameraImageCapture>
#include <QMainWindow>

//using namespace cv;
using namespace std;

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    // Camera
    void startCamera();
    void stopCamera();
    void setCamera(const QCameraInfo &cameraInfo);
    void updateCameraState(QCamera::State state);
    void updateCameraDevice(QAction *action);
    void displayCameraError();

    // Login
    void loginButtonClick();
    void readyForCapture(bool ready);
    void takeImage();
    void processCapturedImage(int requestId, const QImage& img);
    void displayViewfinder();
    void displayCapturedImage();
    void imageSaved(int id, const QString &fileName);
    void displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString);

    // Register
    void registerButtonClick();
    void takeImageButtonClick();
    void cancelButtonClick();

    // etc
    cv::Mat QImage2cvMat(QImage image);

private:
    Ui::Login *ui;

    enum LoginState
    {
        loggingin,
        loginSuccess,
        loginFail,
        registering,
        registerSucess,
        registerFail
    } loginState;

    QCamera *camera = nullptr;
    QCameraImageCapture *imageCapture = nullptr;

    QImageEncoderSettings imageSettings;

    bool isCapturingImage = false;
    bool applicationExiting = false;

};

#endif // LOGIN_H
