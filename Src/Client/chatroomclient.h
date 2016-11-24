#ifndef CHATROOMCLIENT_H
#define CHATROOMCLIENT_H

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
class ChatRoomClient;
}

class ChatRoomClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatRoomClient(QWidget *parent = 0);
    ~ChatRoomClient();

private:
    Ui::ChatRoomClient *ui;
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


#endif // CHATROOMCLIENT_H
