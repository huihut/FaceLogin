#ifndef TRAIN_H
#define TRAIN_H

#include <QThread>
#include <QMessageBox>
#include <string>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "dir.h"

using namespace cv;
using namespace std;

class Train : public QThread
{
    Q_OBJECT
public:
    Train();

    void read_csv(const string&, vector<Mat>&, vector<string>&, char);

//protected:
    void run();
};

#endif // TRAIN_H
