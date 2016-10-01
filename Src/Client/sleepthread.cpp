#include "sleepthread.h"

SleepThread::SleepThread(){}


void SleepThread::usleep(long iSleepTime){
    QThread::usleep(iSleepTime);
    QCoreApplication::processEvents();
}

void SleepThread::sleep(long iSleepTime){
    QThread::sleep(iSleepTime);
    QCoreApplication::processEvents(); //不停地處理事件，以使得程序保持響應。
}

void SleepThread::msleep(long iSleepTime){
    QThread::msleep(iSleepTime);
    QCoreApplication::processEvents();
}

SleepThread::~SleepThread(){

}
