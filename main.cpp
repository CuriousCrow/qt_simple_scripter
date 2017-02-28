#include "mainscripterwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScripterWindow w;
    w.showMaximized();

    return a.exec();
}
