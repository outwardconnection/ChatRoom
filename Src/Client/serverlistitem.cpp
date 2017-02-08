#include "ServerListItem.h"
#include <QFileInfo>
#include <QDir>

ServerItemList::ServerItemList(QWidget *parent):QFrame(parent)
{
    serverIcon = "Resources/Icons/ServerDefaultIcon.png";

    serverNameLabel = new QLabel(this);
    setServerName(QStringLiteral("連線伺服器"));
    serverNameLabel->setAutoFillBackground(true);
    serverAdressLabel = new QLabel(this);
    setServerAdress("");
    serverPortLabel=new QLabel;
    setServerPort(54321);
    leftLayout = new QVBoxLayout;
    leftLayout->addWidget(serverNameLabel);
    leftLayout->addWidget(serverAdressLabel);
    leftLayout->addWidget(serverPortLabel);

    serverIconLabel = new QLabel(this);
    serverIconLabel->setMinimumSize(50,50);
    serverIconLabel->setMaximumSize(100,100);
    setServerIcon(serverIcon);

    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(serverIconLabel);
    mainLayout->addSpacing(100);
    mainLayout->addLayout(leftLayout);
    mainLayout->setMargin(10);
    setMinimumHeight(110);

    palette.setColor(QPalette::Background,QColor(0,0,0,0));
    serverNameLabel->setPalette(palette);
    serverAdressLabel->setPalette(palette);
    serverIconLabel->setPalette(palette);

    setLayout(mainLayout);
    setAutoFillBackground(true);
    setPalette(palette);
}

ServerItemList::~ServerItemList()
{

}

void ServerItemList::mousePressEvent(QMouseEvent *event)
{
    setFrameStyle(QFrame::Box);
    palette.setColor(QPalette::Foreground,Qt::red);
    setPalette(palette);
    palette.setColor(QPalette::WindowText,Qt::black);
    serverNameLabel->setPalette(palette);
    serverAdressLabel->setPalette(palette);
    serverPortLabel->setPalette(palette);
    //serverIcon->setPalette(palette);

    isLocked = true;
    emit pressed(itemId);
}

void ServerItemList::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(itemId);
}

void ServerItemList::unLocked()
{
    setFrameStyle(QFrame::NoFrame);
    //palette.setColor(QPalette::Foreground,Qt::red);
    //setPalette(palette);
    isLocked = false;
}

void ServerItemList::locked()
{
    setFrameStyle(QFrame::Box);
    palette.setColor(QPalette::Foreground,Qt::red);
    setPalette(palette);
    palette.setColor(QPalette::WindowText,Qt::black);
    serverNameLabel->setPalette(palette);
    serverAdressLabel->setPalette(palette);
    serverPortLabel->setPalette(palette);
    isLocked = true;
}

void ServerItemList::setItemId(quint8 id)
{
    itemId = id;
}

void ServerItemList::setServerName(QString name)
{
    serverName = name;
    serverNameLabel->setText(serverName);
}

bool ServerItemList::setServerIcon(QString adress)
{
    QFileInfo *iconInfo = new QFileInfo(adress);
    if(iconInfo->exists())
    {
        serverIcon = adress;
        serverIconLabel->setPixmap(QPixmap(serverIcon).scaled(100,100,Qt::KeepAspectRatio));
        delete iconInfo;
        iconInfo = NULL;
        return true;
    }
    delete iconInfo;
    iconInfo = NULL;
    QDir *iconDir = new QDir("Resources/Icons");
    if(!iconDir->exists())
    {
        if(!iconDir->mkdir(iconDir->currentPath()+"/Resources"))
        {
            qDebug() << QStringLiteral("創建文件夾 :/Resources 失敗");
        }
        if(!iconDir->mkdir(iconDir->currentPath()+"/Resources/Icons"))
        {
            qDebug() << QStringLiteral("創建文件夾 :/Resources/Icons 失敗");
        }
    }

    delete iconDir;
    iconDir = NULL;
    return false;
}

void ServerItemList::clearData()
{
    setServerName(QStringLiteral("連線伺服器"));
    setServerAdress("");
    setServerPort(54321);
    setServerIcon("Resources/Icons/ServerDefaultIcon.png");
}

void ServerItemList::setServerAdress(QString adress)
{
    serverAdress = adress;
    serverAdressLabel->setText(QStringLiteral("連線位址  ")+serverAdress);
}

void ServerItemList::setServerPort(quint16 port)
{
    serverPort = port;
    serverPortLabel->setText(QStringLiteral("通訊埠 　 ")+QString::number(serverPort));
}

quint8 ServerItemList::getItemId()
{
    return itemId;
}

QString ServerItemList::getItemIcon()
{
    return serverIcon;
}

QString ServerItemList::getServerAdress()
{
    return serverAdress;
}

QString ServerItemList::getServerName()
{
    return serverName;
}

quint16 ServerItemList::getServerPort()
{
    return serverPort;
}
