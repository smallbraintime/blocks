#ifndef APP_H
#define APP_H

#include <QWidget>
#include <QMainWindow>

#include "menu.h"

class App : QWidget
{
public:
    App();

private:
     Q_OBJECT

    QMainWindow mWindow;
     Menu* mMenu;
};

#endif // APP_H
