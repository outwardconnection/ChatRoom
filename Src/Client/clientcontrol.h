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
    RelayForm *rf = NULL;
    ChatRoomClient *client = NULL;

signals:

private slots:
    void connectToServer(QString,quint16);

private:
    bool relayFormBulided;
    bool chatRoomClientBulided;
};

#endif // CLIENTCONTROL_H
