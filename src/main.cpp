#include "facelogin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FaceLogin w;
    w.show();

    return a.exec();
}
