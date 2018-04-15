# FaceLogin

Face Login using OpenCV and Qt.

## 部署

### OpenCV

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

在 `src/src.pro` 中配置好 OpenCV 路径

## 数据集

[The AT&T Facedatabase](http://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html)

## 分类器

[lbpcascade_frontalface.xml](https://github.com/opencv/opencv/blob/master/data/lbpcascades/lbpcascade_frontalface.xml)