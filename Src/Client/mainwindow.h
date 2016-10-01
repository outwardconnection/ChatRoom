#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
//#include "SettingDialog/settinginterface.h"
#include "settinginterface.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QAction>

class QFile;
class QString;
class QSettings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void creatActions();
    void creatMenus();
    QMenu *OptionMenu;
    QMenu *HelpMenu;
    QAction *settingAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    quint16 nextBlockSize;
    QAction *sendAction;
    QTcpSocket tcpSocket;
    void closeConnection();
    QString serverAdress;
    settingInterface *setting;
    void readSettings();
    QTextStream settings;
    int serverPort = 54321 ;
    QString userID;
    //QSettings *userSettingsRead;
    bool isConnected = false;

signals:
    void transDeal(quint8,QString,QString);

private slots:
    //int ReadFile();
    void showAbout();
    void newState(QAbstractSocket::SocketState);
    void set();
    void connectToServer();
    void login();
    void connectionCloseByServer();
    void error(QAbstractSocket::SocketError);
    void setServerAdress(QString);
    void sendMessage();
    void connectSuccess();
    void deal();
    void receiveMessage(quint8 requestType,QString userID,QString Message);
    //void test();
    void on_connectPushButton_clicked();
    void on_logoutPushButton_clicked();
};


#endif // MAINWINDOW_H
