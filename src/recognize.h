#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <QThread>

class Recognize : public QThread
{
public:
    Recognize();
protected:
    void run();
};

#endif // RECOGNIZE_H
