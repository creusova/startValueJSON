#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./srcDLL");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


