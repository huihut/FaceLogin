#include "facelogin.h"
#include "ui_facelogin.h"

Q_DECLARE_METATYPE(QCameraInfo)

FaceLogin::FaceLogin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FaceLogin),
    camera(nullptr),
    loginImageCapture(nullptr),
    registerImageCapture(nullptr),
    train(nullptr),
    applicationExiting(false),
    canRecognition(false),
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
    delete train;
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
    connect(registerImageCapture, &QCameraImageCapture::imageSaved, this, &FaceLogin::registerImageSaved);
    connect(registerImageCapture, QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error), this, &FaceLogin::displayRegisterCaptureError);
//    connect(registerImageCapture, &QCameraImageCapture::imageAvailable, this, &FaceLogin::registerReceiveCaptureFrames);
//    connect(registerImageCapture, &QCameraImageCapture::imageCaptured, this, &FaceLogin::processRegisterCapturedImage);

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
    QMessageBox::warning(this, tr("摄像头出错！"), camera->errorString());
}

/***********************************
    Viewfinder 拍照预览
***********************************/

// 登录、注册的拍照是照片在 Viewfinder 预览
void FaceLogin::displayCapturedImage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Viewfinder 预览后回到拍摄状态
void FaceLogin::displayViewfinder()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/***********************************
    Login 登录
***********************************/

// 拍照登录
void FaceLogin::loginTakeImageButtonClick()
{
    loginImageCapture->capture();
}

// 准备拍照
void FaceLogin::readyForLoginCapture(bool ready)
{
    ui->loginButton->setEnabled(ready);
}

// 处理登录拍的照片
void FaceLogin::processLoginCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(ui->viewfinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // 拍的照在 Viewfinder 显示 3 秒
    displayCapturedImage();
    QTimer::singleShot(3000, this, &FaceLogin::displayViewfinder);
}

// 保存登录照片
void FaceLogin::loginImageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    ui->statusBar->showMessage(tr("保存在 \"%1\"").arg(QDir::toNativeSeparators(fileName)));

    if(applicationExiting)
        close();
}

// 登录错误
void FaceLogin::displayLoginCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("拍照出错！"), errorString);
}

// 注册按钮
void FaceLogin::registerButtonClick()
{
    loginState = registering;
    ui->loginButton->setVisible(false);
    ui->registerButton->setVisible(false);
    ui->takeImageButton->setVisible(true);
    ui->cancelButton->setVisible(true);
    ui->usernameLineEdit->setVisible(true);
    ui->statusBar->showMessage(tr("请拍照 %1 次！").arg(entryTimes - entryTimesNow));
}

/***********************************
    Register 注册
***********************************/

void FaceLogin::registerTakeImageButtonClick()
{
    // 拍照次数达到 entryTimes 则完成
    if(entryTimesNow >= entryTimes)
    {
        ui->statusBar->showMessage(tr("图片录入完成！"));
        registerCancelButtonClick();

        // 录入完成，生成 CSV 文件
        Dir::GetDir().CreateCSV();
        return;
    }

    // 只第一次拍照的时候获取注册用户名，之后的拍照都保存在此用户中
    if(entryTimesNow == 0)
    {
        // 保证用户名不为空，并获取用户名
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
    }

    // 拍照
    registerImageCapture->capture();

    // 状态栏提示
    ui->statusBar->showMessage(tr("请拍照 %1 次！").arg(entryTimes - entryTimesNow));
}

// 准备拍照
void FaceLogin::readyForRegisterCapture(bool ready)
{
    ui->loginButton->setEnabled(ready);
}

// 处理录入的照片：拍的照在 Viewfinder 显示 2 秒，切割图片成数据集大小
void FaceLogin::processRegisterCapturedImage(int id, QString str)
{
    Q_UNUSED(id);
    // 加载在系统默认路径下保存的拍摄的照片
    QImage registerImage(str);
    // 获取 PreviewLabel 宽高
    int imgWidth = ui->lastImagePreviewLabel->width();
    int imgHeight = ui->lastImagePreviewLabel->height();
    // 让宽高适应 PreviewLabel，按比例缩放
    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(registerImage.scaled(imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    // 切割图片成数据集大小并保存
    ImageProcessing ip;

    // ImageCutAndSave 中传入 id 为了保证拍摄照片唯一性（不覆盖），可以使用 entryTimesNow，让拍摄照片名字逐个递增，但可能覆盖原来的照片。
    switch (ip.ImageCutAndSave(registerImage, registerUserName, id, userImagePath)) {
    case 0:     // 检测到的人脸数量为：0
        canRecognition = false;
        ui->statusBar->showMessage(tr("无法检测到人脸！"));
        break;
    case 1:     // 检测到的人脸数量为：1
        ++entryTimesNow;
        canRecognition = true;
        // 拍的照在 Viewfinder 显示 2 秒
        displayCapturedImage();
        QTimer::singleShot(2000, this, &FaceLogin::displayViewfinder);
        break;
    default:     // 检测到的人脸数量大于等于2
        canRecognition = false;
        ui->statusBar->showMessage(tr("请只拍一个人！"));
        break;
    }
}

// 保存图片后显示状态栏信息
void FaceLogin::registerImageSaved()
{
    if(canRecognition)
        ui->statusBar->showMessage(tr("【%1】保存在 \"%2\"").arg(entryTimesNow).arg(QDir::toNativeSeparators(userImagePath)));

    if(applicationExiting)
        close();
}

// 注册错误
void FaceLogin::displayRegisterCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("拍照出错！"), errorString);
}

// 取消注册按钮
void FaceLogin::registerCancelButtonClick()
{
    loginState = loggingin;
    entryTimesNow = 0;
    registerUserName.clear();
    userImagePath.clear();
    ui->takeImageButton->setVisible(false);
    ui->cancelButton->setVisible(false);
    ui->usernameLineEdit->setVisible(false);
    ui->loginButton->setVisible(true);
    ui->registerButton->setVisible(true);
    ui->statusBar->showMessage(tr("请拍照登录！"));
}

/***********************************
    Train 训练
***********************************/

void FaceLogin::on_actionStart_Train_triggered()
{
    if(!train)
        train = new Train();

    train->run();
}

/***********************************
    Dataset 数据集
***********************************/

// 在 dataset 文件夹生成 CSV 文件
void FaceLogin::on_actionCreate_CSV_triggered()
{
    if(Dir::GetDir().CreateCSV())
        QMessageBox::information(this, tr("成功"), tr("生成CSV文件成功！"));
    else
        QMessageBox::warning(this, tr("失败"), tr("生成CSV文件失败！"));
}
