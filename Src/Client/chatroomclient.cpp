#include "chatroomclient.h"
#include "sleepthread.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QtGui>
#include <QDialog>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QFileInfo>


ChatRoomClient::ChatRoomClient(QWidget *parent) :QWidget(parent)
{

}

ChatRoomClient::~ChatRoomClient()
{

}

bool ChatRoomClient::connectToServer(QString adress, quint16 port)
{
    showConnectForm(quint8(0));

    return true;
}


void ChatRoomClient::showConnectForm(quint8 formMode)
{
    static bool maked=false;
    if(!maked)
    {
        connectFormLayout = new ConnectFormLayout(this);
    }
    setLayout(connectFormLayout);

    switch (formMode) {
    case quint8(0):
        qDebug() << "Hi";
        connectFormLayout->showConnectingForm();

        break;
    case quint8(1):
        connectFormLayout->showLoginForm();

        break;
    case quint8(2):

        break;
    default:
        qDebug() << QStringLiteral("警告! 在showConnectForm()中傳入未知型態參數");
        break;
    }

}

/*
//測試用
int ChatRoomClient::ReadFile(){
    QString fileLocation = QFileDialog::getOpenFileName(this,QStringLiteral("Open File"),"/home", "Text files (*.txt);;Any files(*.*)");
    QFile file(fileLocation);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream textstream(&file);
        ui->textBrowser->setText(textstream.readAll());
        return 0;
    }else{
        return 1;
    }
}

*/

