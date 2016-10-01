#ifndef CHATROOMSERVER_H
#define CHATROOMSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

class QTextStream;
class QString;

class ClientSocket:public QTcpSocket{
    Q_OBJECT

public:
    ClientSocket(QObject *parent=0);
    void setSocketId(int id);
    void receiveMessage(quint8, QString, QString);
    int Num = 0;
    QString getUserID();

signals:
    void sendMessage(quint8,QString,QString);
    void clientDisconnectedSignal(int);
    void serverInformation(QString info);
    void updateSrvEvnLogSignal(QString);

private slots:
    void readClient();
    void clientDisconnected();
    void newState(QAbstractSocket::SocketState);

private:
    quint16 nextBlockSize;
    QString userID;
    QString message;
    int socketId;
    void updateEvnLog(QString);
    void sendLogSuc();
};

class ChatRoomServer : public QTcpServer{
    Q_OBJECT


public:
    ChatRoomServer(QObject *parent = 0);
    ~ChatRoomServer();
    //std::vector<ClientSocket> clientList;
    ClientSocket *clientList;
    std::vector<QString> ipList;
    std::vector<bool> clientCap_IsEmpty;
    int totalClient;
    void test();

signals:
    void serverInfoToGui(QString info);
    void aleadyClosed();

private:
    void incomingConnection(qintptr socketDescriptor);
    int getClientNum(QString clinetAdress, qintptr );
    int maxPending;
    int countLog;
    QString strOutLog;

private slots:
    void transferMessage(quint8,QString,QString);
    void clientDisconnectedSlot(int);
    void updateEvnLog(QString);
    void endServer();
    void startServer();

};

#endif // CHATROOMSERVER_H
