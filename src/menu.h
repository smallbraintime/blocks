#pragma once

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include "editor.h"

class MainWindow;

class Menu : public QMenuBar {
    Q_OBJECT

public:
    explicit Menu(Editor* editor, QWidget* parent = nullptr);

signals:
    void newTriggered();

private:
    QMenu* m_fileMenu;
    QMenu* m_toolsMenu;
    QMenu* m_helpMenu;

    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_exitAction;

    QAction* m_changeColorAction;

    QAction* m_keyMapAction;
    QAction* m_aboutAction;

    void createFileMenu();
    void createToolsMenu();
    void createHelpMenu();
};
