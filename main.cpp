#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTranslator translator;
//    translator.load(":/langs/main_ru.qm");
//    a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
