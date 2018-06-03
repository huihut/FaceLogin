#ifndef DIR_H
#define DIR_H

#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QDirIterator>

#define DATASET "dataset"
#define SRC "src"
#define MODEL "model"
#define CSV_FILENAME "dataset_csv.txt"

// 文件路径操作类（单例模式）
// 不实现复制构造函数和赋值函数，禁止手动构造、析构、复制、赋值
class Dir
{
private:
    Dir();
    ~Dir();
    Dir(const Dir &);
    Dir & operator = (const Dir &);

    QDir ProgramRootDir();
    QString SrcDir();
    QString DatasetDir();
    QString ModelDir();
    QString CSVPathName();

    QDir programRootDir;
    QString srcDir;
    QString datasetDir;
    QString modelDir;
    QString csvPathName;

public:
    static Dir & GetDir()       // 获取文件路径类的单例方法
    {
        static Dir instanceDir;
        return instanceDir;
    }

    QDir GetProgramRootDir();   // 获取程序根目录
    QString GetSrcDir();        // 获取程序 src 目录
    QString GetDatasetDir();    // 获取程序 dataset 目录
    QString GetModelDir();      // 获取程序 model 目录
    QString GetCSVPathName();   // 获取程序 csv 文件的路径和名字（绝对路径）

    bool CreateCSV();           // 在 dataset 目录下生成 CSV 文件

};



#endif // DIR_H
