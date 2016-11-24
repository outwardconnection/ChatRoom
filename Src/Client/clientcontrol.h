#ifndef CLIENTCONTROL_H
#define CLIENTCONTROL_H

#include "chatroomclient.h"
#include "relayform.h"
#include <QObject>

class ClientControl : public QObject
{
    Q_OBJECT
public:
    explicit ClientControl(QObject *parent = 0);
    ~ClientControl();
    RelayForm *rf;
    ChatRoomClient *client;



signals:

public slots:
};

#endif // CLIENTCONTROL_H
