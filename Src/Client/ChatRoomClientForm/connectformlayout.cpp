#include "connectformlayout.h"
#include <QDebug>

ConnectFormLayout::ConnectFormLayout(QWidget *parent):QVBoxLayout(parent)
{
    buildGui();
}

ConnectFormLayout::~ConnectFormLayout()
{

}

void ConnectFormLayout::showConnectingForm()
{
    connectingLabel->setText(QStringLiteral("連線中..."));
    mainWidget->setLayout(connectingFormLayout);
    return;
}

void ConnectFormLayout::showLoginForm()
{
    mainWidget->setLayout(loginFormLayout);
    return;
}

void ConnectFormLayout::buildGui()
{
    mainWidget = new QWidget;
    connectingLabel = new QLabel;
    connectingLabel->setAlignment(Qt::AlignCenter);
    connectingLabel->setMinimumSize(192,128);
    accountLabel = new QLabel;
    accountLabel->setText(QStringLiteral("帳號："));
    accountLineEdit = new QLineEdit;
    passwordLabel = new QLabel;
    passwordLabel->setText(QStringLiteral("密碼："));
    passwordLineEdit = new QLineEdit;
    registeredLabel = new QLabel;
    registeredLabel->setText(QStringLiteral("註冊帳號"));
    cancelLabel = new QLabel;
    cancelLabel->setText(QStringLiteral("返回"));
    loginButton = new QPushButton;
    loginButton->setText(QStringLiteral("登入"));
    connectingFormLayout = new QVBoxLayout;
    topLayout = new QVBoxLayout;
    accountLayout = new QHBoxLayout;
    passwordLayout = new QHBoxLayout;
    bottomLayout = new QVBoxLayout;
    bottomLabelLayout = new QHBoxLayout;
    loginFormLayout = new QVBoxLayout;

    accountLayout->addWidget(accountLabel);
    accountLayout->addWidget(accountLineEdit);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordLineEdit);
    topLayout->addLayout(accountLayout);
    topLayout->addLayout(passwordLayout);
    bottomLabelLayout->addWidget(registeredLabel);
    bottomLabelLayout->addStretch();
    bottomLabelLayout->addWidget(cancelLabel);
    bottomLayout->addWidget(loginButton);
    bottomLayout->addLayout(bottomLabelLayout);
    connectingFormLayout->addStretch();
    connectingFormLayout->addWidget(connectingLabel);
    connectingFormLayout->addStretch();
    loginFormLayout->addLayout(topLayout);
    loginFormLayout->addLayout(bottomLayout);

    addWidget(mainWidget);

    return;
}
