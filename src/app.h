#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QStackedWidget>

#include "game.h"
#include "menu.h"
class Menu;

class App : QWidget
{
    Q_OBJECT

public:
    App();
    ~App();

private:
    QMainWindow mWindow;
    QStackedWidget *mStackedWidget;
    Menu* mMenu;
    Game* mGame;

private slots:
    void onBackToMenu();
};
