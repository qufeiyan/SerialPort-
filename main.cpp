#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    a.setApplicationName("跌倒检测系统");
    a.setApplicationVersion("0.1");

    a.setStyle(QStyleFactory::create("fusion"));
    MainWindow w;
    w.show();

    return a.exec();
}
