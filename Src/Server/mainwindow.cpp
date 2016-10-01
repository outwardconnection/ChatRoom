#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatroomserver.h"
#include <QDebug>
#include <QString>
#include <QTime>
#include <QDateTime>
#include "sleepthread.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *tmp = new QWidget(this);
    tmp->setLayout(ui->mainLayout);
    setCentralWidget(tmp);
    server = new ChatRoomServer;
    ui->endPushButton->setEnabled(false);
    connect(server,SIGNAL(serverInfoToGui(QString)),
                     this,SLOT(serverInfomation(QString)));
    connect(this,SIGNAL(endServer()),server,SLOT(endServer()));
    connect(server,SIGNAL(aleadyClosed()),this,SLOT(serverAleadyClosed()));
    connect(this,SIGNAL(startServer()),server,SLOT(startServer()));

    setWindowTitle(QStringLiteral("聊天室伺服器"));

    ui->testPushButton->setEnabled(false);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_endPushButton_clicked(){
    emit endServer();
}

void MainWindow::serverInfomation(QString info){
    ui->infoTextBrowser->append(info);
}

void MainWindow::on_testPushButton_clicked(){
    server->test();
}

void MainWindow::on_startPushButton_clicked()
{
    ui->startPushButton->setEnabled(false);
    emit startServer();
    serverRun = true;
    ui->endPushButton->setEnabled(true);

}

void MainWindow::closeEvent(QCloseEvent *event){
    emit endServer();
    forever{
        if(!serverRun){
            event->accept();
            break;
        }
        //SleepThread::msleep(100);
        //QCoreApplication::processEvents();
    }
}

void MainWindow::serverAleadyClosed(){
    ui->endPushButton->setEnabled(false);
    ui->startPushButton->setEnabled(true);
    serverRun = false;
}

