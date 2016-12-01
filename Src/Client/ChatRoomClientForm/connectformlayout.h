#ifndef CONNECTFORMLAYOUT_H
#define CONNECTFORMLAYOUT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>


class ConnectFormLayout : public QVBoxLayout
{
    Q_OBJECT

public:

    explicit ConnectFormLayout(QWidget *parent=0);
    ~ConnectFormLayout();

    QLabel *connectingLabel;
    QLabel *accountLabel;
    QLineEdit *accountLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QLabel *registeredLabel;
    QLabel *cancelLabel;
    QPushButton *loginButton;
    QVBoxLayout *connectingFormLayout;
    QVBoxLayout *topLayout;
    QHBoxLayout *accountLayout;
    QHBoxLayout *passwordLayout;
    QVBoxLayout *bottomLayout;
    QHBoxLayout *bottomLabelLayout;
    QVBoxLayout *loginFormLayout;
    QWidget *mainWidget;

    void buildGui();
    void showConnectingForm();
    void showLoginForm();
};

#endif // CONNECTFORMLAYOUT_H
