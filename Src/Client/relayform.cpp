#include "relayform.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDataStream>              //檢查 creatButton 和 readServerList 創建或添加 Item 方式的不同
#include <QTextStream>
#include <QPalette>

RelayForm::RelayForm(QWidget *parent) :QWidget(parent)
{
    //test(100);

    maxServerAmount = 100;
    lockedItemId=-1;
    totalServerAmount=0;

    serverList = new ServerItemList[maxServerAmount];
    listOrderToId = new int[maxServerAmount+1];
    for(int i=0;i<30;i++)
    {
        listOrderToId[i]=0;
    }

    mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    for(unsigned int i=0;i<maxServerAmount;i++)
    {
        serverList[i].beUsing=false;
    }

    readServerLists();
    resize(700,430);
    setLayout(mainLayout);
    showServerListForm();
    setMinimumWidth(this->width());

}

RelayForm::~RelayForm()
{
    delete[] serverList;
    delete[] listOrderToId;
}

void RelayForm::closeEvent(QCloseEvent *event)
{
    writeServerLists();
    qDebug() << QStringLiteral("關閉RelayForm");
    event->accept();
}

void RelayForm::connected()
{
    this->close();
}

bool RelayForm::readServerLists()
{
    const quint8 propety_serverName=0;
    const quint8 propety_serverAdress=1;
    const quint8 propety_serverPort=2;
    const quint8 propety_serverIconAdress=3;

    QFileInfo *fInfo = new QFileInfo("Records/serverlists.lst");
    QFile *fLists;
    if(fInfo->exists())
    {
        fLists = new QFile("Records/serverlists.lst");
        delete fInfo;
    }
    else
    {
        QDir recordDir("Records");
        if(!recordDir.exists())
        {
            if(!recordDir.mkdir(recordDir.currentPath()+"/Records"))
            {
                //qDebug() << recordDir.currentPath();
                qDebug() << QStringLiteral("創建文件夾 :/Records 失敗");
                delete fInfo;
                return false;
            }
        }
        fLists = new QFile("Records/serverlists.lst");
        fLists->open(QIODevice::WriteOnly);
        fLists->close();
        delete fInfo;
        delete fLists;
        return true;
    }
    if(fLists->open(QIODevice::ReadOnly))
    {
        QDataStream in(fLists);
        //in.setCodec("UTF-8");
        in.setVersion(QDataStream::Qt_5_7);
        while(!in.atEnd())
        {
            quint8 porpety=255;
            quint16 portValue;
            QString value("");
            in >> porpety;
            switch(porpety)
            {
            case propety_serverName:
                totalServerAmount++;
                in >> value;
                serverList[totalServerAmount-1].setServerName(value);
                serverList[totalServerAmount-1].beUsing=true;
                serverList[totalServerAmount-1].order=totalServerAmount;
                listOrderToId[totalServerAmount]=totalServerAmount-1;
                //qDebug() << "totalServerAmount = " << totalServerAmount;
                //qDebug() << "listOrderToId = " << listOrderToId[totalServerAmount];
                //qDebug() << QStringLiteral("ServerName : ") + value;
                break;
            case propety_serverAdress:
                in >> value;
                serverList[totalServerAmount-1].setServerAdress(value);
                //qDebug() << QStringLiteral("ServerAdress : ") + value;
                break;
            case propety_serverPort:
                in >> portValue;
                serverList[totalServerAmount-1].setServerPort(portValue);
                //qDebug() << QStringLiteral("ServerPort : ") + portValue;
                break;
            case propety_serverIconAdress:
                in >> value;
                serverList[totalServerAmount-1].setServerIcon(value);
                //qDebug() << QStringLiteral("ServerIcon : ") + value;
                break;
            default:
                qDebug() << QStringLiteral("readRecord()中的porpety為未知類型");
            }
        }
            //getRecordContent(in.readLine());
    }
    else
    {
        delete fLists;
        return false; //有檔案但讀取失敗時 才傳回false ★ω★
    }
    //qDebug() << totalServerAmount;
    fLists->close();
    delete fLists;

    return true;
}

bool RelayForm::writeServerLists()
{
    const quint8 propety_serverName=0;
    const quint8 propety_serverAdress=1;
    const quint8 propety_serverPort=2;
    const quint8 propety_serverIconAdress=3;

    QFileInfo fInfo("Records/serverlists");
    QFile *fLists;
    if(fInfo.exists())
    {
        fLists = new QFile("Records/serverlists.lst");
    }
    else
    {
        QDir listsDir("Records");
        if(!listsDir.exists())
        {
            if(!listsDir.mkdir(listsDir.currentPath()+"/Records"))
            {
                qDebug() << QStringLiteral("創建文件夾 /Records 失敗");
                //delete fLists;
                return false;
            }
        }
        fLists = new QFile("Records/serverlists.lst");
    }

    if(fLists->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
       QDataStream out(fLists);
       out.setVersion(QDataStream::Qt_5_7);
       for(int i=1;i<=totalServerAmount;i++)
       {
           //qDebug() << "i="+QString::number(i) << " "
           //         << "listOrderToId[i]="+QString::number(listOrderToId[i]);
           if(serverList[listOrderToId[i]].beUsing)
           {
               out << propety_serverName << serverList[listOrderToId[i]].getServerName()
                   << propety_serverAdress << serverList[listOrderToId[i]].getServerAdress()
                   << propety_serverPort << serverList[listOrderToId[i]].getServerPort()
                   << propety_serverIconAdress << serverList[listOrderToId[i]].getItemIcon();
           }
       }

    }
    else
    {
        delete fLists;
        qDebug() << QStringLiteral("開啟 Records/serverlists.lst 失敗");
        return false;
    }

    fLists->flush();
    fLists->close();

    return true;
}

void RelayForm::editServer()
{
    //qDebug() << "editServer()";
    if(!(serverPortLineEdit_EditServerForm->text().toInt()>=0 && serverPortLineEdit_EditServerForm->text().toInt()<=65535))
    {
        qDebug() << QStringLiteral("請將通訊埠的值設於0到65535之間");
        return;
    }

    if(makeNewServerItem)
    {
        int i=0;
        while(serverList[i].beUsing){
            i++;
        }

        serverCapLayout_ServerListForm->addWidget(&serverList[i]);
        serverList[i].setItemId(i);
        serverList[i].beUsing=true;
        serverList[i].locked();
        serverList[i].show();
        connect(&serverList[i],SIGNAL(pressed(int)),this,SLOT(changeLockedItem(int)));
        updateServerAmount(++totalServerAmount);
        serverList[i].order=totalServerAmount;
        listOrderToId[serverList[i].order]=i;
        scrollArea_ServerListForm->ensureVisible(0,serverCapWidget_ServerListForm->height());
        changeLockedItem(i);

        serverCapWidget_ServerListForm->resize(serverCapWidget_ServerListForm->width(),
                                               (serverCapWidget_ServerListForm->height()+121));

        makeNewServerItem=false;
    }

    serverList[lockedItemId].setServerPort(serverPortLineEdit_EditServerForm->text().toInt());
    serverList[lockedItemId].setServerName(serverNameLineEdit_EditServerForm->text());
    serverList[lockedItemId].setServerAdress(serverAdressLineEdit_EditServerForm->text());

    //qDebug() << "creatID" <<  listOrderToId[totalServerAmount];

    hideEditServerForm();
    showServerListForm();

}
void RelayForm::cancelEditServer()
{
    //qDebug() << "cancelEditServer()";
    hideEditServerForm();
    showServerListForm();
}

void RelayForm::on_creatButton_ServerListForm_clicked()
{
    //qDebug()<<"creat";
    if(totalServerAmount >= maxServerAmount){
        qDebug() << QStringLiteral("已達創建最上限");
        return;
    }else{
        /*
        int i=0;
        while(serverList[i].beUsing){
            i++;
        }
        */
        makeNewServerItem=true;
    }
    on_editButton_ServerListForm_clicked();
}

void RelayForm::on_editButton_ServerListForm_clicked()
{
    //if(lockedItemId!=-1)
    //{
        hideServerListForm();
        showEditServerForm();
    //}
}

void RelayForm::on_deleteButton_ServerListForm_clicked()
{
    //qDebug()<<"delete";
    //qDebug() << lockedItemId;
    if(lockedItemId != -1)
    {
        serverList[lockedItemId].hide();
        serverList[lockedItemId].unLocked();
        serverList[lockedItemId].clearData();
        serverList[lockedItemId].beUsing=false;
        quint32 deletedListOrder=serverList[lockedItemId].order;
        serverList[lockedItemId].order=0;
        //qDebug() << "deletedListOrder " + QString::number(deletedListOrder);
        for(int i=deletedListOrder+1;i<=totalServerAmount;i++)
        {
            if(serverList[listOrderToId[i]].beUsing)
            {
                //qDebug() << "listOrderToId[i] "+QString::number(listOrderToId[i]);
                serverList[listOrderToId[i]].order -=1;
                listOrderToId[i-1]=serverList[listOrderToId[i]].getItemId();
            }
        }
        lockedItemId=-1;
        updateServerAmount(--totalServerAmount);
        serverCapWidget_ServerListForm->resize(serverCapWidget_ServerListForm->width(),
                                               (serverCapWidget_ServerListForm->height()-121));
    }
}

void RelayForm::on_enterButton_ServerListForm_clicked()
{
    //serverCapWidget_ServerListForm->setStyleSheet(QString::fromUtf8("border:1px solid blue"));
    //qDebug()<<"enter";
    emit connectToServer(serverList[lockedItemId].getServerAdress(),
                         serverList[lockedItemId].getServerPort());
    this->hide();
}

void RelayForm::showServerListForm()
{
    static bool maked=false;

    if(!maked)
    {

        //qDebug() << "showServerListForm";

        creatButton_ServerListForm = new QPushButton;
        editButton_ServerListForm = new QPushButton;
        deleteButton_ServerListForm = new QPushButton;
        enterButton_ServerListForm = new QPushButton;
        topLayout_ServerListForm = new QHBoxLayout;
        scrollArea_ServerListForm = new ScrollAreaModify;
        serverCapWidget_ServerListForm = new QWidget(scrollArea_ServerListForm);
        serverCapLayout_ServerListForm = new QVBoxLayout;
        serverListFormLayout = new QVBoxLayout;
        serverListFormWidget = new QWidget;
        bottomLayout_ServerListForm = new QHBoxLayout;

        creatButton_ServerListForm->setText(QStringLiteral("新增"));
        editButton_ServerListForm->setText(QStringLiteral("編輯"));
        deleteButton_ServerListForm->setText(QStringLiteral("刪除"));
        enterButton_ServerListForm->setText(QStringLiteral("連線"));
        serverItemAmountLabel_ServerListForm = new QLabel(QString::number(totalServerAmount)+"/"+QString::number(maxServerAmount));

        topLayout_ServerListForm->addWidget(creatButton_ServerListForm);
        topLayout_ServerListForm->addWidget(editButton_ServerListForm);
        topLayout_ServerListForm->addWidget(deleteButton_ServerListForm);
        topLayout_ServerListForm->addWidget(enterButton_ServerListForm);

        serverCapWidget_ServerListForm->setMinimumWidth(480);
        connect(scrollArea_ServerListForm,SIGNAL(resizeSignal(int,int)),this,SLOT(scrollAreaResized(int,int)));
        serverCapLayout_ServerListForm->setMargin(5);

        for(unsigned int i=0;i<maxServerAmount;i++)
        {
            serverList[i].setMinimumWidth(scrollArea_ServerListForm->width());
            serverList[i].setMinimumHeight(114);
            serverList[i].setMaximumHeight(114);
        }

        for(unsigned int i=0;i<totalServerAmount;i++)  //應用totalServerAmount
        {
            serverCapLayout_ServerListForm->addWidget(&serverList[i]);
            serverList[i].setItemId(i);
            serverList[i].beUsing=true;
            //serverList[i].setMinimumWidth(scrollArea_ServerListForm->width());
            connect(&serverList[i],SIGNAL(pressed(int)),this,SLOT(changeLockedItem(int)));
        }
        serverCapWidget_ServerListForm->setLayout(serverCapLayout_ServerListForm);
        scrollArea_ServerListForm->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_ServerListForm->setWidget(serverCapWidget_ServerListForm);


        serverItemAmountLabel_ServerListForm->setAlignment(Qt::AlignVCenter);
        serverItemAmountLabel_ServerListForm->setAlignment(Qt::AlignRight);

        bottomLayout_ServerListForm->addWidget(serverItemAmountLabel_ServerListForm);

        serverListFormLayout->addLayout(topLayout_ServerListForm);
        serverListFormLayout->addWidget(scrollArea_ServerListForm);
        serverListFormLayout->addLayout(bottomLayout_ServerListForm);

        connect(creatButton_ServerListForm,SIGNAL(clicked()),this,SLOT(on_creatButton_ServerListForm_clicked()));
        connect(editButton_ServerListForm,SIGNAL(clicked()),this,SLOT(on_editButton_ServerListForm_clicked()));
        connect(deleteButton_ServerListForm,SIGNAL(clicked()),this,SLOT(on_deleteButton_ServerListForm_clicked()));
        connect(enterButton_ServerListForm,SIGNAL(clicked()),this,SLOT(on_enterButton_ServerListForm_clicked()));

        serverListFormWidget->setLayout(serverListFormLayout);

        maked = true;
    }
    mainLayout->addWidget(serverListFormWidget);
    serverListFormWidget->show();
}

void RelayForm::showEditServerForm()
{
    static bool maked=false;

    if(!maked)
    {
        label_1_EditServerForm = new QLabel;
        label_2_EditServerForm = new QLabel;
        label_3_EditServerForm = new QLabel;
        //label_4_EditForm = new QLabel;
        serverAdressLineEdit_EditServerForm = new QLineEdit;
        serverNameLineEdit_EditServerForm = new QLineEdit;
        serverPortLineEdit_EditServerForm = new QLineEdit;
        certainButton_EditServerForm = new QPushButton;
        cancelButton_EditServerForm = new QPushButton;
        contextLayout_EditServerForm = new QVBoxLayout;
        bottomLayout_EditServerForm = new QHBoxLayout;
        editServerFormLayout = new QVBoxLayout;
        editServerFormWidget = new QWidget;

        label_1_EditServerForm->setText(QStringLiteral("伺服器名稱"));
        label_2_EditServerForm->setText(QStringLiteral("伺服器位址"));
        label_3_EditServerForm->setText(QStringLiteral("Port"));
        certainButton_EditServerForm->setText(QStringLiteral("確定"));
        cancelButton_EditServerForm->setText(QStringLiteral("取消"));

        connect(certainButton_EditServerForm,SIGNAL(clicked()),this,SLOT(editServer()));
        connect(cancelButton_EditServerForm,SIGNAL(clicked()),this,SLOT(cancelEditServer()));

        contextLayout_EditServerForm->addWidget(label_1_EditServerForm);
        contextLayout_EditServerForm->addWidget(serverNameLineEdit_EditServerForm);
        contextLayout_EditServerForm->addWidget(label_2_EditServerForm);
        contextLayout_EditServerForm->addWidget(serverAdressLineEdit_EditServerForm);
        contextLayout_EditServerForm->addWidget(label_3_EditServerForm);
        contextLayout_EditServerForm->addWidget(serverPortLineEdit_EditServerForm);

        bottomLayout_EditServerForm->addWidget(certainButton_EditServerForm);
        bottomLayout_EditServerForm->addWidget(cancelButton_EditServerForm);

        editServerFormLayout->addLayout(contextLayout_EditServerForm);
        editServerFormLayout->addLayout(bottomLayout_EditServerForm);
        editServerFormWidget->setLayout(editServerFormLayout);

        maked=true;
    }

    if(makeNewServerItem)
    {
        serverNameLineEdit_EditServerForm->setText(QStringLiteral("連線伺服器"));
        serverAdressLineEdit_EditServerForm->setText("");
        serverPortLineEdit_EditServerForm->setText(QString::number(54321));
    }
    else
    {
        if(lockedItemId!=-1)
        {
            serverNameLineEdit_EditServerForm->setText(serverList[lockedItemId].getServerName());
            serverAdressLineEdit_EditServerForm->setText(serverList[lockedItemId].getServerAdress());
            serverPortLineEdit_EditServerForm->setText(QString::number(serverList[lockedItemId].getServerPort()));
        }
        else
        {
            hideEditServerForm();
            showServerListForm();
            return;
        }
    }

    mainLayout->addWidget(editServerFormWidget);
    editServerFormWidget->show();
}

void RelayForm::changeLockedItem(int newLockedItem)
{
    if(lockedItemId!=-1 && lockedItemId!=newLockedItem)
    {
        serverList[lockedItemId].unLocked();
    }
    //qDebug() << "newLockedItemID = " + QString::number(newLockedItem);
    //qDebug() << serverList[newLockedItem].order
    //         << listOrderToId[serverList[newLockedItem].order];

    lockedItemId=newLockedItem;
    return;
}

void RelayForm::showLoginForm()
{

}

void RelayForm::hideServerListForm()
{
    mainLayout->removeWidget(serverListFormWidget);
    serverListFormWidget->hide();
}

void RelayForm::hideEditServerForm()
{
    editServerFormWidget->hide();
}

void RelayForm::hideLoginForm()
{

}

void RelayForm::updateServerAmount(quint32 newAmount)
{
    totalServerAmount = newAmount;
    serverItemAmountLabel_ServerListForm->setText(QString::number(newAmount)+
                                   "/"+QString::number(maxServerAmount));
}

void RelayForm::scrollAreaResized(int w, int h)
{
    serverCapWidget_ServerListForm->resize(w-23,serverCapWidget_ServerListForm->height());
}

void RelayForm::test(int k)
{
    const quint8 propety_serverName=0;
    const quint8 propety_serverAdress=1;
    const quint8 propety_serverPort=2;
    const quint8 propety_serverIconAdress=3;

    QFile f("Records/serverlists.lst");
    f.open(QIODevice::WriteOnly | QIODevice::Append);
    QDataStream out(&f);
    //out << propety_serverName << QStringLiteral("test")+QString::number(i);
    //out << propety_serverAdress << QStringLiteral("localHost");

    for(int i=0;i<k;i++)
    {
        out << propety_serverName << QStringLiteral("Test ")+QString::number(i+1)
            << propety_serverAdress << QStringLiteral("140.120.126.131")
            << propety_serverPort << quint16(64525);

    }


    f.flush();
    f.close();

}

