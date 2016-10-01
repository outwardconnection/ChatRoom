#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class ChatRoomServer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void endServer();
    void startServer();

private slots:
    void on_endPushButton_clicked();
    void serverInfomation(QString info);
    void on_testPushButton_clicked();
    void on_startPushButton_clicked();
    void serverAleadyClosed();

private:
    Ui::MainWindow *ui;
    ChatRoomServer *server = 0;
    bool serverRun = false;
};

#endif // MAINWINDOW_H
