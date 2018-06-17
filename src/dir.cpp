#include "dir.h"

Dir::Dir() :
    programRootDir(ProgramRootDir()),
    srcDir(SrcDir()),
    datasetDir(DatasetDir()),
    modelDir(ModelDir()),
    csvPathName(GetDatasetDir() + QDir::toNativeSeparators("/") + CSV_FILENAME)
{
}

Dir::~Dir()
{

}

// -------- 获取文件路径 --------

QDir Dir::ProgramRootDir() const
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

QString Dir::SrcDir() const
{
    QDir dir = GetProgramRootDir();
    dir.cd(SRC);
    return dir.absolutePath();
}

QString Dir::DatasetDir() const
{
    QDir dir = GetProgramRootDir();
    dir.cd(DATASET);
    return dir.absolutePath();
}

QString Dir::ModelDir() const
{
    QDir dir = GetProgramRootDir();
    dir.cd(MODEL);
    return dir.absolutePath();
}

QString Dir::CSVPathName() const
{
    return GetDatasetDir() + QDir::toNativeSeparators("/") + CSV_FILENAME;
}

// -------- 返回文件路径 --------

QDir Dir::GetProgramRootDir() const
{
    return programRootDir;
}
QString Dir::GetSrcDir() const
{
    return srcDir;
}
QString Dir::GetDatasetDir() const
{
    return datasetDir;
}

QString Dir::GetModelDir() const
{
    return modelDir;
}

QString Dir::GetCSVPathName() const
{
    return csvPathName;
}

// -------- 生成 CSV 文件 --------

// 在 dataset 目录下生成 CSV 文件
bool Dir::CreateCSV()
{
    QString datasetdir(GetDatasetDir());
    QString datasetPath, datasetName;
    char separator = ',';

    // 文件迭代器：获取指定类型的数据集文件
    QDirIterator it(datasetdir, QStringList() << "*.pgm" << "*.png" << "*.jpg", QDir::Files, QDirIterator::Subdirectories);
    if(!it.hasNext())
    {
        qDebug() << "Can't find dataset!\n";
        return false;
    }

    // 创建及打开 CSV 文件
    QFile file(GetCSVPathName());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Can't open CSV file!\n";
        return false;
    }
    // 文件写的文本流
    QTextStream csv_ts(&file);

    // 文件迭代器中有指定数据集文件则依次迭代
    while (it.hasNext())
    {
        datasetPath = it.next();
        datasetName = datasetPath.section(QDir::toNativeSeparators("/"), -2, -2);
        csv_ts << datasetPath << separator << datasetName << "\n";
    }
    file.close();

    return true;
}

