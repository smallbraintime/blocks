#include "menu.h"

#include <QCoreApplication>

Menu::Menu(QWidget* parent) : QMenuBar(parent) {
    createFileMenu();
    createEditMenu();
    createHelpMenu();
    addMenu(m_fileMenu);
    addMenu(m_editMenu);
    addMenu(m_helpMenu);
}

void Menu::createFileMenu() {
    m_openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                               tr("&Open"), this);
    connect(m_openAction, &QAction::triggered, this, &Menu::newTriggered);
    m_newAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                              tr("&New"), this);
    m_saveAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                               tr("&Save"), this);
    m_exitAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit),
                               tr("&Exit"), this);
    connect(m_exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    m_fileMenu = new QMenu(tr("&File"));
    m_fileMenu->addActions({m_openAction, m_newAction, m_saveAction, m_exitAction});
}

void Menu::createEditMenu() {
    m_undoAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo),
                               tr("&Undo"), this);
    m_redoAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo),
                               tr("&Redo"), this);
    m_modifyAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties),
                                tr("&Modify"), this);
    m_editMenu = new QMenu(tr("&Edit"));
    m_editMenu->addActions({m_undoAction, m_redoAction, m_modifyAction});
}

void Menu::createHelpMenu() {
    m_keyMapAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::InputKeyboard),
                                     tr("&Key Map"), this);
    m_aboutAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
                                tr("&About"), this);
    m_helpMenu = new QMenu(tr("&Help"));
    m_helpMenu->addActions({m_keyMapAction, m_aboutAction});
}
