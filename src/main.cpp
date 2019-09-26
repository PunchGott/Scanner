#include "mainwindow.h"
#include <QApplication>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly))
        a.setStyleSheet(styleFile.readAll());
    else qDebug() << "File isn't open!" << endl;
    MainWindow w;



    return a.exec();
}
