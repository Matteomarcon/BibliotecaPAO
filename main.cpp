#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon appIcon(":/icone/biblioteca.png");  // Se usi le risorse Qt (qrc)
    w.setWindowIcon(appIcon);
    w.showMaximized();
    qRegisterMetaType<Media*>();
    return a.exec();
}
