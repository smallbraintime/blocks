#include <QApplication>
#include <QMainWindow>

#include "app.h"

int main(int argc, char *argv[]) {
    // QSurfaceFormat format;
    // format.setVersion(4, 1);
    // format.setProfile(QSurfaceFormat::CoreProfile);
    // format.setSamples(8);
    // QSurfaceFormat::setDefaultFormat(format);

    App app (argc, argv);
    return app.exec();
}
