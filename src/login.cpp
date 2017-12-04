#include "login.h"
#include "ui_login.h"

#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMessageBox>
#include <QtWidgets>

Q_DECLARE_METATYPE(QCameraInfo)

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    loginState = loggingin;
    ui->takeImageButton->setVisible(false);
    ui->cancelButton->setVisible(false);
    ui->usernameLineEdit->setVisible(false);

    // Camera devices:
    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
        if (cameraInfo == QCameraInfo::defaultCamera())
            videoDeviceAction->setChecked(true);

        ui->menuDevices->addAction(videoDeviceAction);
    }
    // Camera
    connect(ui->actionStartCamera, &QAction::triggered, this, &Login::startCamera);
    connect(ui->actionStopCamera, &QAction::triggered, this, &Login::stopCamera);
    // Login
    connect(ui->loginButton, &QPushButton::clicked, this, &Login::loginButtonClick);
    connect(ui->registerButton, &QPushButton::clicked, this, &Login::registerButtonClick);
    // Register
    connect(ui->takeImageButton, &QPushButton::clicked, this, &Login::takeImageButtonClick);
    connect(ui->cancelButton, &QPushButton::clicked, this, &Login::cancelButtonClick);


    connect(videoDevicesGroup, &QActionGroup::triggered, this, &Login::updateCameraDevice);

    setCamera(QCameraInfo::defaultCamera());
}

Login::~Login()
{
    delete ui;
}

/***********************************
    Camera
***********************************/

void Login::setCamera(const QCameraInfo &cameraInfo)
{
    delete imageCapture;
    delete camera;

    camera = new QCamera(cameraInfo);

    connect(camera, &QCamera::stateChanged, this, &Login::updateCameraState);
    connect(camera, QOverload<QCamera::Error>::of(&QCamera::error), this, &Login::displayCameraError);

    imageCapture = new QCameraImageCapture(camera);

    camera->setViewfinder(ui->viewfinder);

    updateCameraState(camera->state());

    connect(imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &Login::readyForCapture);
    connect(imageCapture, &QCameraImageCapture::imageCaptured, this, &Login::processCapturedImage);
    connect(imageCapture, &QCameraImageCapture::imageSaved, this, &Login::imageSaved);
    connect(imageCapture, QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error), this, &Login::displayCaptureError);

    camera->start();
}

void Login::startCamera()
{
    camera->start();
}

void Login::stopCamera()
{
    camera->stop();
}

void Login::updateCameraState(QCamera::State state)
{
    switch (state) {
    case QCamera::ActiveState:
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
        ui->actionSettings->setEnabled(true);
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        ui->actionSettings->setEnabled(false);
        break;
    }
}

void Login::updateCameraDevice(QAction *action)
{
    setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

void Login::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera Error"), camera->errorString());
}

/***********************************
    Login
***********************************/

void Login::loginButtonClick()
{
    isCapturingImage = true;
    imageCapture->capture();
}

void Login::readyForCapture(bool ready)
{
    ui->loginButton->setEnabled(ready);
}

void Login::takeImage()
{
    isCapturingImage = true;
    imageCapture->capture();
}

void Login::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(ui->viewfinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // Display capture image for 3 seconds.
    displayCapturedImage();
    QTimer::singleShot(3000, this, &Login::displayViewfinder);
}

void Login::displayViewfinder()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Login::displayCapturedImage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Login::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    ui->statusBar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

    isCapturingImage = false;
    if(applicationExiting)
        close();
}

void Login::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    isCapturingImage = false;
}

/***********************************
    Register
***********************************/

void Login::registerButtonClick()
{
    loginState = registering;
    ui->loginButton->setVisible(false);
    ui->registerButton->setVisible(false);
    ui->takeImageButton->setVisible(true);
    ui->cancelButton->setVisible(true);
    ui->usernameLineEdit->setVisible(true);

}

void Login::takeImageButtonClick()
{

}

void Login::cancelButtonClick()
{
    loginState = loggingin;

    ui->takeImageButton->setVisible(false);
    ui->cancelButton->setVisible(false);
    ui->usernameLineEdit->setVisible(false);
    ui->loginButton->setVisible(true);
    ui->registerButton->setVisible(true);
}
