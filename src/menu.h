#pragma once

#include <QMenuBar>
#include <QMenu>
#include <QAction>

class Menu : public QMenuBar {
    Q_OBJECT

public:
    explicit Menu(QWidget* parent = nullptr);

signals:
    void newTriggered();

private:
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_helpMenu;

    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_exitAction;

    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_modifyAction;

    QAction* m_keyMapAction;
    QAction* m_aboutAction;

    void createFileMenu();
    void createEditMenu();
    void createHelpMenu();
};
