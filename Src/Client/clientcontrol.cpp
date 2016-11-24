#include "clientcontrol.h"
#include <windows.h>

ClientControl::ClientControl(QObject *parent) : QObject(parent)
{
    qDebug() << "Hellow";
    rf = new RelayForm;
    //client = new ChatRoomClient;
    rf->show();


}

ClientControl::~ClientControl()
{
    delete rf;
    //delete client;

}

