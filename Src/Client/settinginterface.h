#ifndef SETTINGINTERFACE_H
#define SETTINGINTERFACE_H

#include <QDialog>
#include <QString>
class QLabel;
class QPushButton;
class QLineEdit;
class QSettings;

class UserSettings{

public:
    QString userID;
    QString connectedServer;
};

class settingInterface : public QDialog
{
    Q_OBJECT

public:
     settingInterface(QWidget *parent = 0);
    ~settingInterface();
     QPushButton *changeButton;
     void getSettings(const QString userID);

private:

    QLabel *label;
    QLineEdit *serverAdressLineEdit;
    QPushButton *cancelButton;
    QSettings *userSettings;
    void updateGui();
    UserSettings currentUser;

private slots:
    void endSetting();
    void changeServerAdress();
signals:
    void serverAdressChanged(QString);
};

#endif // SETTINGINTERFACE_H
