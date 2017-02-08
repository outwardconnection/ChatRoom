#include "clientcontrol.h"
#include <windows.h>

ClientControl::ClientControl(QObject *parent) : QObject(parent)
{
    chatRoomClientBulided=false;
    relayFormBulided=false;

    rf = new RelayForm;
    relayFormBulided=true;
    connect(rf,SIGNAL(connectToServer(QString,quint16)),this,SLOT(connectToServer(QString,quint16)));
    rf->show();

    //client = new ChatRoomClient;
    //chatRoomClientBulided=true;
    //client->show();


}

ClientControl::~ClientControl()
{
    if(rf != NULL)
    {
        delete rf;
        rf = NULL;
    }
    if(client != NULL)
    {
        delete client;
        client = NULL;
    }

}

void ClientControl::connectToServer(QString serverAdress,quint16 serverPort)
{

    rf->hide();
    if(client == NULL)
    {
        client = new ChatRoomClient;
        chatRoomClientBulided=true;

    }
    client->show();

    if(client->connectToServer(serverAdress,serverPort))
    {

    }
    else
    {

    }
}

