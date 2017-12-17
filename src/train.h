#ifndef TRAIN_H
#define TRAIN_H

#include <QThread>

class Train : public QThread
{
public:
    Train();
protected:
    void run();
};

#endif // TRAIN_H
