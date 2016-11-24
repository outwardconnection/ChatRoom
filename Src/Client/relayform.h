#ifndef RELAYFORM_H
#define RELAYFORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "ServerListItem.h"
#include "scrollareamodify.h"
#include <QCloseEvent>
#include <QVector>
#include <QString>

class RelayForm : public QWidget
{
    Q_OBJECT

public:
    explicit RelayForm(QWidget *parent = 0);
    ~RelayForm();
    QVBoxLayout *mainLayout;
    //---ServerListForm
    QWidget *serverListFormWidget;
    QVBoxLayout *serverListFormLayout;
    QVBoxLayout *serverCapLayout_ServerListForm;
    QHBoxLayout *topLayout_ServerListForm;
    QVBoxLayout *scrollLayout_ServerListForm;
    QHBoxLayout *bottomLayout_ServerListForm;
    ScrollAreaModify *scrollArea_ServerListForm;
    QWidget *serverCapWidget_ServerListForm;
    QPushButton *creatButton_ServerListForm;
    QPushButton *editButton_ServerListForm;
    QPushButton *deleteButton_ServerListForm;
    QPushButton *enterButton_ServerListForm;
    QLabel *serverItemAmountLabel_ServerListForm;
    //---

    //---EditServerForm
    QLineEdit *serverNameLineEdit_EditServerForm;
    QLineEdit *serverAdressLineEdit_EditServerForm;
    QLineEdit *serverPortLineEdit_EditServerForm;
    QPushButton *certainButton_EditServerForm;
    QPushButton *cancelButton_EditServerForm;
    QLabel *label_1_EditServerForm;
    QLabel *label_2_EditServerForm;
    QLabel *label_3_EditServerForm;
    //QLabel *label_4_EditForm;
    QVBoxLayout *contextLayout_EditServerForm;
    QHBoxLayout *bottomLayout_EditServerForm;
    QVBoxLayout *editServerFormLayout;
    QWidget *editServerFormWidget;

    //---

    //---LoginForm


    //---

    bool readServerLists();
    bool writeServerLists();
    void updateServerAmount(quint32);
    void closeEvent(QCloseEvent *event);

private:
    ServerItemList *serverList;
    int *listOrderToId;
    bool makeNewServerItem=false;
    quint32 lockedItemId=-1;
    unsigned int totalServerAmount=0;
    quint32 maxServerAmount;
    void test(int k);
    void hideServerListForm();
    void hideEditServerForm();
    void hideLoginForm();
    void showServerListForm();
    void showEditServerForm();
    void showLoginForm();

private slots:
    void changeLockedItem(int newLockedItem);
    void scrollAreaResized(int,int);
    void on_creatButton_ServerListForm_clicked();
    void on_editButton_ServerListForm_clicked();
    void on_deleteButton_ServerListForm_clicked();
    void on_enterButton_ServerListForm_clicked();
    void editServer();
    void cancelEditServer();
    void connected();

signals:

    void connectToServer(QString,quint16);

};

#endif // RELAYFORM_H
