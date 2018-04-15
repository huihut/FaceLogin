# FaceLogin
Face Login using OpenCV and Qt.

## Deploy

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