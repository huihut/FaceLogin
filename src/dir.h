#ifndef DIR_H
#define DIR_H

#include <QDir>
#include <QApplication>

QDir programRootDir();
QString DataDir();
const char * qdir_c_data();

#endif // DIR_H
