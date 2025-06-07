#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QMainWindow>
#include <QApplication>
#include <QStackedWidget>

#include "menu.h"
#include "editor.h"

class App : public QApplication {
public:
    explicit App(int& argc, char** argv);
    static App* instance();

private:
    QMainWindow m_window;
    QStackedWidget* m_mainWidgets;
    Menu* m_menu;
    Editor* m_editor;
    QLabel* m_startPage;

public slots:
    void openEditor();
};
