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
    Dir(const Dir &) = delete;
    Dir & operator = (const Dir &) = delete;

    QDir ProgramRootDir() const;
    QString SrcDir() const;
    QString DatasetDir() const;
    QString ModelDir() const;
    QString CSVPathName() const;

public:
    static Dir & GetDir()               // 获取文件路径类的单例方法
    {
        static Dir instanceDir;
        return instanceDir;
    }

    QDir GetProgramRootDir() const;     // 获取程序根目录
    QString GetSrcDir() const;          // 获取程序 src 目录
    QString GetDatasetDir() const;      // 获取程序 dataset 目录
    QString GetModelDir() const;        // 获取程序 model 目录
    QString GetCSVPathName() const;     // 获取程序 csv 文件的路径和名字（绝对路径）

    bool CreateCSV();                   // 在 dataset 目录下生成 CSV 文件

private:
    const QDir programRootDir;
    const QString srcDir;
    const QString datasetDir;
    const QString modelDir;
    const QString csvPathName;
};



#endif // DIR_H
