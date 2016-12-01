#ifndef CHATROOMCLIENT_H
#define CHATROOMCLIENT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QTcpSocket>
#include <QTextStream>
#include "ChatRoomClientForm/connectformlayout.h"

class QFile;
class QString;
class QSettings;

namespace Ui {
class ChatRoomClient;
}

class ChatRoomClient : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoomClient(QWidget *parent = 0);
    ~ChatRoomClient();
    void showConnectForm(quint8 formMode=0);
    ConnectFormLayout *connectFormLayout;
    QString serverAdrss;
    quint32 serverPort;


private:

signals:

public slots:
    bool connectToServer(QString,quint16);

private slots:


};

#endif // CHATROOMCLIENT_H
