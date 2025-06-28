#include <QApplication>
#include <QMainWindow>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    // QSurfaceFormat format;
    // format.setVersion(4, 1);
    // format.setProfile(QSurfaceFormat::CoreProfile);
    // format.setSamples(4);
    // QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
