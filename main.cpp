#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Axmouth");
    QCoreApplication::setApplicationName("QResizer");
#if QT_VERSION < 0x060000
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
