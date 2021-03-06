#include "startwindow.h"

#include <QFile>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly))
        a.setStyleSheet(styleFile.readAll());
    else qDebug() << "File isn't open!" << endl;
    StartWindow w;
    w.show();



    return a.exec();
}

