#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QMainWindow>
#include <QApplication>
#include <QStackedWidget>

#include "menu.h"
#include "editor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget* m_mainWidgets;
    Menu* m_menu;
    Editor* m_editor;
    QLabel* m_startPage;
    QSurfaceFormat* m_surfaceFormat;

public slots:
    void openEditor();
};
