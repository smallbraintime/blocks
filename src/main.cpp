#include <QApplication>

#include "app.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    App app;
    return a.exec();
}
