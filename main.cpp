#include "mainwindow.h"
#include <QApplication>
#include "myopenglwidget.h"
int main(int argc, char *argv[])
{

    //设置OpenGL版本
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
