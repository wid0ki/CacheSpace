#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(QString(":/icons/cpu.png")));
    MainWindow MW_w;
    MW_w.show();

    return a.exec();
}
