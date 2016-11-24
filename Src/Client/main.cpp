#include "clientcontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientControl client;

    return a.exec();
}
