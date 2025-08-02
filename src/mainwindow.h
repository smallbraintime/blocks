#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QStackedWidget>

#include "menu.h"
#include "editor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    Editor* m_editor;
    Menu* m_menu;
    QLabel* m_startPage;
    QStackedWidget* m_mainWidgets;

public slots:
    void openEditor();
};
