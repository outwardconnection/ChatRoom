#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QtGui>
#include <QDialog>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QFileInfo>
#include "sleepthread.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Chat Room"));
    readSettings();
    creatActions();
    creatMenus();
    ui->textBrowser->setText(QStringLiteral(""));
    setting = 0;
    ui->sendButton->setEnabled(false);
    userID = "Visitors";
    ui->IDlineEdit->setText(QStringLiteral("Visitors"));
    ui->logoutPushButton->setEnabled(false);
    isConnected = false;

    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(login()));
    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(sendMessage()));
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
    connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(connectionCloseByServer()));
    connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(deal()));
    connect(this,SIGNAL(transDeal(quint8,QString,QString)),this,SLOT(receiveMessage(quint8,QString,QString)));
    connect(&tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(newState(QAbstractSocket::SocketState)));

    setMinimumWidth(470);
    setMinimumHeight(580);

    QWidget *mainLayoutContainer = new QWidget(this);
    mainLayoutContainer->setLayout(ui->mainLayout);
    setCentralWidget(mainLayoutContainer);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_connectPushButton_clicked(){
    userID = ui->IDlineEdit->text();
    if(!serverAdress.isEmpty()){
        connectToServer();
        ui->connectPushButton->setEnabled(false);
    }else{
        ui->statusLabel->setText(QStringLiteral("沒有設置伺服器IP"));
    }

}

void MainWindow::readSettings(){
    QFileInfo fi("UserSettings.ini");

    if(!fi.isFile()){
        qDebug() << QStringLiteral("創建檔案");
        QFile *file = new QFile("UserSettings.ini");
        file = new QFile("UserSettings.ini");
        file->open(QIODevice::WriteOnly); //建立
        file->close();
        delete file;
        return;
    }
    QSettings userSettingsRead("UserSettings.ini",QSettings::IniFormat);
    serverAdress = userSettingsRead.value("IP").toString();
    qDebug() << QStringLiteral("目前欲連線至 : ") << serverAdress;

}

void MainWindow::creatActions(){
    settingAction = new QAction(QStringLiteral("設定"),this);
    settingAction->setStatusTip(QStringLiteral("開啟設定介面"));
    connect(settingAction,SIGNAL(triggered()),this,SLOT(set()));

    aboutAction = new QAction(QStringLiteral("關於"),this);
    aboutAction->setStatusTip(QStringLiteral("程式資訊"));
    connect(aboutAction,SIGNAL(triggered()),this,(SLOT(showAbout())));

    aboutQtAction = new QAction(QStringLiteral("關於 Qt"),this);
    aboutQtAction->setStatusTip(QStringLiteral("Qt資訊"));
    connect(aboutQtAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));

    sendAction = new QAction(QStringLiteral("傳送訊息"),this);
    sendAction->setShortcut(QString("Enter"));
    addAction(sendAction);
    connect(sendAction,SIGNAL(triggered()),this,SLOT(sendMessage()));
}

void MainWindow::showAbout(){
    QMessageBox message(QMessageBox::NoIcon,QStringLiteral("關於"),"ChatRoom ver0.0.1",QMessageBox::Ok);
    message.exec();
}

void MainWindow::creatMenus(){
    OptionMenu = ui->menuBar->addMenu(QStringLiteral("選項"));
    OptionMenu->addAction(settingAction);

    HelpMenu = ui->menuBar->addMenu(QStringLiteral("幫助"));
    HelpMenu->addAction(aboutAction);
    HelpMenu->addAction(aboutQtAction);
}

void MainWindow::set(){
    if(!setting){
        setting = new settingInterface(this);
        setting->getSettings(userID);
        QObject::connect(setting,SIGNAL(serverAdressChanged(QString)),this,SLOT(setServerAdress(QString)));

    }
    setting->show();
    setting->raise();
    setting->activateWindow();
}

void MainWindow::connectToServer(){
    qDebug() << "connectToServer()";
    userID = ui->IDlineEdit->text();
    tcpSocket.connectToHost(serverAdress,serverPort);
    ui->statusLabel->setText(QStringLiteral("連線中..."));
    ui->connectPushButton->setEnabled(false);
    ui->IDlineEdit->setEnabled(false);
    nextBlockSize = 0;
}

void MainWindow::connectSuccess(){
    qDebug() << "connectSuccess()";
    ui->sendButton->setEnabled(true);
    ui->logoutPushButton->setEnabled(true);
    ui->statusLabel->setText(QStringLiteral("連線成功"));
    isConnected = true;
}

void MainWindow::connectionCloseByServer(){
    qDebug() << "connectionCloseByServer()";
    if(nextBlockSize!=0xFFF)
        //ui->statusLabel->setText(QStringLiteral("錯誤:伺服端切斷連線"));
    closeConnection();
    isConnected = false;
}

void MainWindow::error(QAbstractSocket::SocketError){
    qDebug() << "error()";
    ui->statusLabel->setText(tcpSocket.errorString());
    qDebug() << tcpSocket.errorString();
    closeConnection();
}

void MainWindow::newState(QAbstractSocket::SocketState){
    qDebug() << QStringLiteral("新的state狀態 : ")+QString::number(tcpSocket.state());
}

void MainWindow::on_logoutPushButton_clicked(){
    closeConnection();
    ui->logoutPushButton->setEnabled(false);
}


void MainWindow::closeConnection(){
    qDebug() << "closeConnection()";
    tcpSocket.close();
    ui->connectPushButton->setEnabled(true);
    ui->logoutPushButton->setEnabled(false);
    ui->IDlineEdit->setEnabled(true);
    ui->sendButton->setEnabled(false);
}

void MainWindow::setServerAdress(QString newAdress = ""){

    if(newAdress.isEmpty()){
        QMessageBox message(QMessageBox::Warning ,QStringLiteral("!!!"),QStringLiteral("沒有設置伺服器位址"),QMessageBox::Ok);
        message.exec();
        return;
    }else{
        if(isConnected){
            closeConnection();
            qDebug() << QStringLiteral("closeConnection()");
        }
        serverAdress = newAdress;
        QSettings *userSettingsWrite = new QSettings("UserSettings.ini", QSettings::IniFormat);
        userSettingsWrite->setValue("IP",serverAdress);
        delete userSettingsWrite;
        connectToServer();
        qDebug() << serverAdress;
        setting->reject();
    }

}

void MainWindow::sendMessage(){
    if(!isConnected){
        return;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << quint8('S') << ui->MessageTextEdit->toPlainText();
    qDebug() << ui->MessageTextEdit->toPlainText();
    qDebug() << block;
    out.device()->seek(0);
    tcpSocket.write(block);
    ui->MessageTextEdit->setText("");
}

void MainWindow::deal(){
    //qDebug() << QStringLiteral("deal()中的state ")+QString::number(tcpSocket.state());
    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_5_7);

        if(nextBlockSize == 0){
            if(tcpSocket.bytesAvailable() < sizeof(quint16))
                return;
            in >> nextBlockSize;
        }

        if(nextBlockSize == 0xFFFF){
            nextBlockSize = 0;
            return;
        }
        if(tcpSocket.bytesAvailable() < nextBlockSize)
            return;

        quint8 requestType;

        in >> requestType;
        char c = requestType;
        qDebug() << QStringLiteral("接收模式")+c;
        if(requestType == quint8('E')){   //錯誤
            return;
        }else{
            if(requestType == quint8('L')){   //登入成功
                qDebug() << QStringLiteral("接收到成功登入訊息");
                nextBlockSize = 0;
                connectSuccess();
                return;
            }else{
                if(requestType == quint8('S')){   //傳送訊息
                    quint8 requestType_2;
                    in >> requestType_2;
                    if(requestType_2 == quint8('A')){
                        QString Message;
                        in >> Message;
                        emit transDeal(requestType_2,"System",Message);
                        return;
                    }else{
                        if(requestType_2 == quint8('M')){
                            QString sendUserID;
                            QString Message;
                            in >> sendUserID >> Message;
                            emit transDeal(requestType_2,sendUserID,Message);
                            return;
                        }
                    }
                }else{
                    //do nothing 也代表不符服所有預設 requestType 類型
                    qDebug() << QStringLiteral("未知requestType類型 : ") << requestType;
                }
            }
        }
        qDebug() << QStringLiteral("執行至deal()最底，requestType類型是 : ") << requestType;
}

void MainWindow::login(){
    //qDebug() << QStringLiteral("login()中的state ")+QString::number(tcpSocket.state());
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << quint8('L') << userID ;
    tcpSocket.write(block);
}

void MainWindow::receiveMessage(quint8 requestType = quint8('A'),QString sendUserID = QStringLiteral("系統"),QString message = QStringLiteral("")){
    if (requestType == quint8('A')){
        ui->textBrowser->append(QStringLiteral("伺服器公告 : ")+message);
        return;
    }else{
        if(requestType == quint8('M')){
            ui->textBrowser->append(sendUserID+QStringLiteral(" : ")+message);
         return;
        }
    }
    return;
}
/*
//測試用
int MainWindow::ReadFile(){
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
