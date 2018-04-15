#include "facelogin.h"
#include "ui_facelogin.h"

Q_DECLARE_METATYPE(QCameraInfo)

FaceLogin::FaceLogin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FaceLogin),
    camera(nullptr),
    loginImageCapture(nullptr),
    registerImageCapture(nullptr),
    isLoginCapturingImage(false),
    isRegisterCapturingImage(false),
    applicationExiting(false),
    entryTimes(ENTRY_TIMES),  // 录入人脸拍照次数
    entryTimesNow(0)
{
    ui->setupUi(this);

    loginState = loggingin;

    // Button & StatusBar
    ui->takeImageButton->setVisible(false);
    ui->cancelButton->setVisible(false);
    ui->usernameLineEdit->setVisible(false);
    ui->statusBar->showMessage(tr("请拍照登录！"));

    // Camera devices:
    videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
        if (cameraInfo == QCameraInfo::defaultCamera())
            videoDeviceAction->setChecked(true);

        ui->menuDevices->addAction(videoDeviceAction);
    }
    connect(videoDevicesGroup, &QActionGroup::triggered, this, &FaceLogin::updateCameraDevice);

    // Camera
    connect(ui->actionStartCamera, &QAction::triggered, this, &FaceLogin::startCamera);
    connect(ui->actionStopCamera, &QAction::triggered, this, &FaceLogin::stopCamera);

    // Login
    connect(ui->loginButton, &QPushButton::clicked, this, &FaceLogin::loginTakeImageButtonClick);
    connect(ui->registerButton, &QPushButton::clicked, this, &FaceLogin::registerButtonClick);

    // Register
    connect(ui->takeImageButton, &QPushButton::clicked, this, &FaceLogin::registerTakeImageButtonClick);
    connect(ui->cancelButton, &QPushButton::clicked, this, &FaceLogin::registerCancelButtonClick);

    // setCamera
    setCamera(QCameraInfo::defaultCamera());
}

FaceLogin::~FaceLogin()
{
    delete videoDeviceAction;
    delete videoDevicesGroup;
    delete loginImageCapture;
    delete registerImageCapture;
    delete camera;
    delete ui;
}

/***********************************
    Camera 相机
***********************************/

void FaceLogin::setCamera(const QCameraInfo &cameraInfo)
{
    delete camera;
    delete loginImageCapture;
    delete registerImageCapture;

    camera = new QCamera(cameraInfo);
    loginImageCapture = new QCameraImageCapture(camera);
    registerImageCapture = new QCameraImageCapture(camera);

    // camera
    connect(camera, &QCamera::stateChanged, this, &FaceLogin::updateCameraState);
    connect(camera, QOverload<QCamera::Error>::of(&QCamera::error), this, &FaceLogin::displayCameraError);

    // loginImageCapture
    connect(loginImageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &FaceLogin::readyForLoginCapture);
    connect(loginImageCapture, &QCameraImageCapture::imageCaptured, this, &FaceLogin::processLoginCapturedImage);
    connect(loginImageCapture, &QCameraImageCapture::imageSaved, this, &FaceLogin::loginImageSaved);
    connect(loginImageCapture, QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error), this, &FaceLogin::displayLoginCaptureError);

    // registerImageCapture
    connect(registerImageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &FaceLogin::readyForRegisterCapture);
    connect(registerImageCapture, &QCameraImageCapture::imageSaved, this, &FaceLogin::processRegisterCapturedImage);
    connect(registerImageCapture, QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error), this, &FaceLogin::displayRegisterCaptureError);
//    connect(registerImageCapture, &QCameraImageCapture::imageAvailable, this, &FaceLogin::registerReceiveCaptureFrames);
//    connect(registerImageCapture, &QCameraImageCapture::imageCaptured, this, &FaceLogin::processRegisterCapturedImage);
//    connect(registerImageCapture, &QCameraImageCapture::imageSaved, this, &FaceLogin::registerImageSaved);

    // camera
    camera->setViewfinder(ui->viewfinder);
    updateCameraState(camera->state());
    camera->start();
}

void FaceLogin::startCamera()
{
    camera->start();
}

void FaceLogin::stopCamera()
{
    camera->stop();
}

void FaceLogin::updateCameraState(QCamera::State state)
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

void FaceLogin::updateCameraDevice(QAction *action)
{
    setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

void FaceLogin::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera Error"), camera->errorString());
}

/***********************************
    Login 登录 Register 注册
***********************************/

void FaceLogin::displayViewfinder()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FaceLogin::displayCapturedImage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

/***********************************
    Login 登录
***********************************/

void FaceLogin::loginTakeImageButtonClick()
{
    isLoginCapturingImage = true;
    loginImageCapture->capture();
}

void FaceLogin::readyForLoginCapture(bool ready)
{
    ui->loginButton->setEnabled(ready);
}

void FaceLogin::processLoginCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(ui->viewfinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // Display capture image for 3 seconds.
    displayCapturedImage();
    QTimer::singleShot(3000, this, &FaceLogin::displayViewfinder);
}

void FaceLogin::loginImageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    ui->statusBar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

    isLoginCapturingImage = false;
    if(applicationExiting)
        close();
}

void FaceLogin::displayLoginCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    isLoginCapturingImage = false;
}

void FaceLogin::registerButtonClick()
{
    loginState = registering;
    ui->loginButton->setVisible(false);
    ui->registerButton->setVisible(false);
    ui->takeImageButton->setVisible(true);
    ui->cancelButton->setVisible(true);
    ui->usernameLineEdit->setVisible(true);
    ui->statusBar->showMessage(tr("请拍照 %1 次！").arg(entryTimes));
}

/***********************************
    Register 注册
***********************************/

void FaceLogin::registerTakeImageButtonClick()
{
    if(registerUserName.isEmpty())
    {
        QString userName = ui->usernameLineEdit->text();
        if(userName.isEmpty())
        {
            ui->statusBar->showMessage(tr("用户名不能为空！"));
            return;
        }
        registerUserName = userName;
    }
    if(entryTimesNow >=  entryTimes)
    {
        ui->statusBar->showMessage(tr("图片录入完成！"));
        entryTimesNow = 0;
        registerUserName.clear();
        registerCancelButtonClick();
        return;
    }

    isLoginCapturingImage = true;

    loginImageCapture->capture();

    ui->statusBar->showMessage(tr("请拍照 %1 次！").arg(entryTimes-entryTimesNow));
}

void FaceLogin::readyForRegisterCapture(bool ready)
{
    ui->loginButton->setEnabled(ready);
}

// 处理录入的照片：拍的照在 Viewfinder 显示 3 秒，切割图片成数据集大小
void FaceLogin::processRegisterCapturedImage(int id, QString str)
{
    Q_UNUSED(id);
    QImage registerImage(str);
    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(registerImage));

    // 切割图片成数据集大小并保存
    ImageProcessing ip;
    switch (ip.ImageCutAndSave(registerImage, id)) {
    case 0:
        ui->statusBar->showMessage(tr("无法检测到人脸！"));
        break;
    case 1:
        ++entryTimesNow;
        // 拍的照在 Viewfinder 显示 3 秒
        displayCapturedImage();
        QTimer::singleShot(5000, this, &FaceLogin::displayViewfinder);
        break;
    default:
        ui->statusBar->showMessage(tr("请只拍摄一个人！"));
        break;
    }
}

void FaceLogin::registerImageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    ui->statusBar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

    isRegisterCapturingImage = false;
    if(applicationExiting)
        close();
}

void FaceLogin::displayRegisterCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    isRegisterCapturingImage = false;
}

void FaceLogin::registerCancelButtonClick()
{
    loginState = loggingin;
    ui->takeImageButton->setVisible(false);
    ui->cancelButton->setVisible(false);
    ui->usernameLineEdit->setVisible(false);
    ui->loginButton->setVisible(true);
    ui->registerButton->setVisible(true);
    ui->statusBar->showMessage(tr("请拍照登录！"));
}
