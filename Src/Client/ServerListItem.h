#ifndef SERVERITEMLIST_H
#define SERVERITEMLIST_H

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QPalette>

class ServerItemList : public QFrame
{
    Q_OBJECT

public:
    ServerItemList(QWidget *parent=0);
    ~ServerItemList();
    QHBoxLayout *mainLayout;//水平排列
    QVBoxLayout *leftLayout;
    QLabel *serverIconLabel;
    QLabel *serverNameLabel;
    QLabel *serverAdressLabel;
    QLabel *serverPortLabel;

    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void setItemId(quint8);
    quint8 getItemId();
    bool setServerIcon(QString);
    QString getItemIcon();
    void setServerAdress(QString);
    QString getServerAdress();
    void setServerPort(quint16);
    quint16 getServerPort();
    void setServerName(QString);
    QString getServerName();
    void locked();
    void clearData();
    quint32 order=1;
    bool beUsing=false;

private:
    bool isLocked = false;
    int itemId;
    QPalette palette;
    QString serverName;
    QString serverAdress;
    QString serverIcon;
    quint16 serverPort=54321;

signals:
    void pressed(int);
    void doubleClicked(int);

public slots:
    void unLocked();

private slots:


};

#endif // SERVERITEMLIST_H
