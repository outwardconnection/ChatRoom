#include "chatroomserver.h"
#include <QDebug>
#include <QDataStream>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "sleepthread.h"

ChatRoomServer::ChatRoomServer(QObject *parent):QTcpServer(parent){

    maxPending = 50;
    totalClient = 0;
    countLog = 0;

    setMaxPendingConnections(maxPending);
    updateEvnLog( QStringLiteral("設定連線上限")+QString::number(maxPendingConnections()) );
    //clientList.resize(maxPendingConnections());
    clientList = new ClientSocket[maxPendingConnections()];
    clientCap_IsEmpty.resize(maxPendingConnections());
    for(int i=0;i<maxPendingConnections();i++){
        clientCap_IsEmpty.at(i) = true;
    }
    ipList.resize(maxPendingConnections());

}

void ChatRoomServer::startServer(){

    if(!listen(QHostAddress::Any,54321)){
        updateEvnLog(QStringLiteral("監聽port失敗"));
        close();
    }else{
        updateEvnLog(QStringLiteral("監聽port成功"));
        updateEvnLog(QStringLiteral("開使伺服器連線"));
    }
}

ChatRoomServer::~ChatRoomServer(){

}

void ChatRoomServer::incomingConnection(qintptr socketDescriptor){

    if(totalClient >= maxPending){
        updateEvnLog( QStringLiteral("連線人數已達上限") );
        return;
    }

    int socketNum = 0;
    for(int i=0;i<clientCap_IsEmpty.size();i++){ //若無連線過，則尋找空的clientList容器配給他
        if(clientCap_IsEmpty.at(i)){
            clientList[i].setSocketDescriptor(socketDescriptor);
            clientCap_IsEmpty[i] = false;
            ipList.at(i) = clientList[i].peerAddress().toString();
            totalClient++;
            socketNum = i;
            clientList[socketNum].Num = socketNum;
            break;
        }
    }
    updateEvnLog( QStringLiteral("發現新連線 IP : ") + ipList.at(socketNum) +
                  QStringLiteral(" Socket : ") + QString::number(socketDescriptor) +
                  QStringLiteral(" 連線座位為")+QString::number(socketNum) );

    connect(&clientList[socketNum],SIGNAL(sendMessage(quint8,QString,QString)),
            this,SLOT(transferMessage(quint8,QString,QString)));
    connect(&clientList[socketNum],SIGNAL(clientDisconnectedSignal(int)),
            this,SLOT(clientDisconnectedSlot(int)));
    connect(&clientList[socketNum],SIGNAL(updateSrvEvnLogSignal(QString)),
                     this,SLOT(updateEvnLog(QString)));
    QDateTime tTmpCurrentTime;
    QString tmp = tTmpCurrentTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
            " " + clientList[socketNum].getUserID() + QStringLiteral(" 登入");
    transferMessage(quint8('A'),QStringLiteral("伺服器公告"),tmp);

    updateEvnLog( QStringLiteral("現在連線數量")+QString::number(totalClient) );
    //==-----------
}

int ChatRoomServer::getClientNum(QString clinetAdress,qintptr socketDescriptor){
    return 0;
}

void ChatRoomServer::clientDisconnectedSlot(int socketNum){
    disconnect(&clientList[socketNum],SIGNAL(clientDisconnectedSignal(int)),
            this,SLOT(clientDisconnectedSlot(int)));

    qDebug() << "ChatRoomServer::clientDisconnectedSlot()";

    clientCap_IsEmpty.at(socketNum) = true;
    ipList.at(socketNum) = "";
    disconnect(&clientList[socketNum],SIGNAL(sendMessage(quint8,QString,QString)),
            this,SLOT(transferMessage(quint8,QString,QString)));
    disconnect(&clientList[socketNum],SIGNAL(updateSrvEvnLogSignal(QString)),
                     this,SLOT(updateEvnLog(QString)));

    updateEvnLog( QStringLiteral("SocketNum: ") + QString::number(socketNum) + QStringLiteral(" ->刪除連線") );
    updateEvnLog( QStringLiteral("現在連線人數") + QString::number(--totalClient) );
}

void ChatRoomServer::transferMessage(quint8 type,QString sender=QStringLiteral("伺服器公告"),QString message=""){

    for(int i =0;i<clientCap_IsEmpty.size();i++){
        if(!clientCap_IsEmpty.at(i)){
            clientList[i].receiveMessage(type,sender,message);
        }
    }
}

void ChatRoomServer::updateEvnLog(QString info){
    QDateTime tTmpCurrentTime;
    QString sCurrentTime =
            tTmpCurrentTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    info = sCurrentTime + " " + info;

    emit serverInfoToGui(info);
    qDebug() << info;
    strOutLog = strOutLog+info+"\n";
    countLog++;
    if(countLog >= 10){
        QFile fEvnLog("EventLog.log");
        if(!fEvnLog.open(QIODevice::Append | QIODevice::WriteOnly)){
            qDebug() << QStringLiteral("無法開啟EventLog.log");
            strOutLog +=QStringLiteral("無法開啟EventLog.log")+"\n";
            return;
        }
        QTextStream streamOutLog(&fEvnLog);
        streamOutLog.setCodec("UTF-8");
        streamOutLog << strOutLog;
        streamOutLog.flush();
        fEvnLog.close();
        countLog = 0;
        strOutLog = QString("");
    }

}

void ChatRoomServer::endServer(){
    totalClient = 0;

    updateEvnLog(QStringLiteral("關閉伺服器"));

    if(countLog > 0){
        QFile fEvnLog("EventLog.log");
        if(!fEvnLog.open(QIODevice::WriteOnly)){
            qDebug() << QStringLiteral("無法開啟EventLog.log");
            strOutLog +=QStringLiteral("無法開啟EventLog.log")+"\n";
            return;
        }
        QTextStream streamOutLog(&fEvnLog);
        streamOutLog.setCodec("UTF-8");
        streamOutLog << strOutLog;
        streamOutLog.flush();
        fEvnLog.close();
        countLog = 0;
        strOutLog = QString("");
    }

    for(int i=0 ;i<clientCap_IsEmpty.size();i++){
        if(!clientCap_IsEmpty.at(i)){
            clientList[i].disconnectFromHost();
            clientList[i].close();
            clientList[i].abort();
        }
    }
    close();   
    emit aleadyClosed();
}

void ChatRoomServer::test(){
    return;
}

//-------ClientSocket(QSocket)--------

ClientSocket::ClientSocket(QObject *parent):QTcpSocket(parent){

    connect(this,SIGNAL(readyRead()),this,SLOT(readClient()));
    connect(this,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));
    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(newState(QAbstractSocket::SocketState)));

    nextBlockSize = 0;
}

void ClientSocket::setSocketId(int id){
    socketId = id;
}

void ClientSocket::readClient(){
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_7);

    if(nextBlockSize == 0){
        if(bytesAvailable() < sizeof(quint16)){
            return;
        }
        in >> nextBlockSize;
    }
    if(bytesAvailable() < nextBlockSize){
        return;
    }

    quint8 requestType;
    in >> requestType;

    if(requestType == quint8('L')){
        in >> userID;

        updateEvnLog(userID+QStringLiteral("登入"));

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_7);
        out << quint16(0) << quint8('L');
        write(block);
        updateEvnLog(QStringLiteral("傳回")+userID+QStringLiteral("登入成功訊息"));

    }else{
        if(requestType == quint8('S')){
            if(userID.isEmpty()){
                updateEvnLog("發現未登入者傳送訊息");
            }else{

                in >> message;
                updateEvnLog(userID+" : "+message);

                emit sendMessage(quint8('M'),userID,message);

            }
        }else{
            updateEvnLog( QStringLiteral("客戶端發送未知要求型態")+QString::number(requestType));
        }
    }

}

void ClientSocket::sendLogSuc(){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << quint8('L');
    write(block);
    updateEvnLog(QStringLiteral("傳回")+userID+QStringLiteral("登入成功訊息"));
}

void ClientSocket::receiveMessage(quint8 type,QString sender,QString message){

    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << quint8('S');

    if(type == quint8('A')){ //系統公告
        out << quint8('A') << message;
    }else{
        if(type == quint8('M')){  //使用者訊息
            out << quint8('M') << sender << message;
        }else{
            //doNothing
        }
    }

    write(block);
}

void ClientSocket::newState(QAbstractSocket::SocketState){
    qDebug() << userID+QStringLiteral(" 的statue ")+QString::number(state());
}

QString ClientSocket::getUserID(){
    return userID;
}

void ClientSocket::updateEvnLog(QString info){
    emit updateSrvEvnLogSignal(info);
}

void ClientSocket::clientDisconnected(){
    qDebug() << "clientDisconnected()";

    int i = 0;

    emit clientDisconnectedSignal(Num);

}
