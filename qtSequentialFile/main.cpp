#include "mainwindow.h"

#include <QApplication>
//#include "TestController.h"
//#include "TestSequential.h"

int main(int argc, char *argv[])
{
    //string filename = "/home/theflilux/qtSequentialFile/Alumno.txt";
    //testLoad_add_Alumno(filename);
    //TestController::testRun(filename);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
