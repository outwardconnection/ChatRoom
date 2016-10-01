#include <QtGui>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QSettings>
#include "settinginterface.h"


settingInterface::settingInterface(QWidget *parent) :QDialog(parent)
{
    label = new QLabel(QStringLiteral("伺服器位址"));
    serverAdressLineEdit = new QLineEdit;
    changeButton = new QPushButton(QStringLiteral("變更"));
    cancelButton = new QPushButton(QStringLiteral("取消"));

    QObject::connect(changeButton,SIGNAL(clicked()),this,SLOT(changeServerAdress()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(endSetting()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(serverAdressLineEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(changeButton);
    bottomLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    setWindowTitle(QStringLiteral("設定"));
    this->resize(473, 92);
    //setFixedHeight(sizeHint().height());
    //setFixedWidth(sizeHint().width());

}

settingInterface::~settingInterface()
{
}

void settingInterface::getSettings(const QString ReceiveUserID){
    QSettings userSettingsRead("UserSettings.ini",QSettings::IniFormat);

    qDebug() << "getSettings";
    currentUser.userID = ReceiveUserID;

    qDebug() << "getSettings before =";
    currentUser.connectedServer = userSettingsRead.value("IP").toString();
    qDebug() << "getSettings before updateGui";
    updateGui();
    qDebug() << "getSettings end";
    return;
}

void settingInterface::endSetting(){
    this->reject();
}

void settingInterface::changeServerAdress(){
    emit serverAdressChanged(serverAdressLineEdit->text());
}

void settingInterface::updateGui(){
    serverAdressLineEdit->setText(currentUser.connectedServer);
}
