#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Axmouth");
    QCoreApplication::setApplicationName("QResizer");
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
    MainWindow w;
    w.show();
    return a.exec();
}
