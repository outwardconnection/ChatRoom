#ifndef SLEEPTHREAD_H
#define SLEEPTHREAD_H
#include <QThread>
#include <QCoreApplication>

class SleepThread :public QThread{

    Q_OBJECT

    void run(){}

public:
    SleepThread();
    ~SleepThread();
    static void usleep(long);
    static void sleep(long);
    static void msleep(long);
};

#endif // SLEEPTHREAD_H

