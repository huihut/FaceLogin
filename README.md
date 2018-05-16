# FaceLogin

使用 OpenCV 和 Qt 实现人脸（刷脸）登录

日常消遣开发中 _\(:з」∠)\_

## 部署

### 克隆 FaceLogin

```
git clone git@github.com:huihut/FaceLogin.git
```

### 配置 OpenCV

#### MacOS

安装 OpenCV 和 pkg-config

```
brew update
brew tap homebrew/homebrew-science
brew install opencv
brew install pkg-config
```

查看 OpenCV 的 include 和 lib 安装路径

```
pkg-config --libs opencv
pkg-config --cflags opencv
```

在 `FaceLogin/src/src.pro` 中配置好 OpenCV 的 include、lib 路径

### 下载数据集

下载 [AT&T Facedatabase](http://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html) 数据集，保存在与 `src` 文件夹同级目录的 `dataset` 文件夹中，如下文件树。


## 文件树

```
.
└── FaceLogin
    ├── FaceLogin.pro                       # 整个工程的 pro 文件，使用 Qt 打开它
    ├── README.md                           # FaceLogin 的 Readme 文件
    ├── dataset                             # 数据集文件夹
    │   ├── dataset_csv.txt                 # 数据集的 CSV 文件，录入人脸数据（注册）后生成
    │   ├── README
    │   ├── s1                              # 数据集每个人的名字
    │   │   ├── 1.pgm                       # 人脸数据（图片）
    │   │   └── ...
    │   └── ...
    ├── model                               # 模型文件夹
    │   └── lbpcascade_frontalface.xml      # 前置人脸分类器
    ├── release                             # 生成的可执行文件
    └── src                                 # 源码文件夹
        ├── ...
        ├── src.pro                         # 源码的 pro 文件，在这里配置 OpenCV 的 include、lib 路径
        └── ...
```

## 数据集

[AT&T Facedatabase](http://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html)

## 分类器

[lbpcascade_frontalface.xml](https://github.com/opencv/opencv/blob/master/data/lbpcascades/lbpcascade_frontalface.xml)