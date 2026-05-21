# include "mainwindow.h"
#include <QApplication>
#include <cstdlib>

int main(int argc, char *argv[]) {

    setenv("QT_MAC_WANTS_LAYER", "1", 1);


    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus);

    QApplication a(argc, argv);


    a.setStyle("Fusion");

    MainWindow w;
    w.show();
    return a.exec();
}