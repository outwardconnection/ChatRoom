#include "mainwindow.h"
#include "relayform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    RelayForm form;
    form.show();

    return a.exec();
}
