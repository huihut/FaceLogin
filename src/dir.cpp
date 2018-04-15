#include "dir.h"

#define DATA "data"

QDir programRootDir()
{
    QDir dir(QApplication::applicationDirPath());

#if defined(Q_OS_WIN)
    if (dir.dirName().toLower() == "debug"
               || dir.dirName().toLower() == "release"
               || dir.dirName().toLower() == "bin")
           dir.cdUp();

#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS") {
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }
#endif

    return dir;
}

QString DataDir()
{
    QDir dir = programRootDir();
    dir.cd(DATA);
    return dir.absolutePath();
}

const char * qdir_c_data()
{
    QDir dir = programRootDir();

    // Set the images path
    dir.cd(DATA);
    QString data_path = dir.absolutePath();
    std::string str = data_path.toStdString();
    return str.c_str();
}
