#include "menu.h"

#include <QCoreApplication>

#include "app.h"

Menu::Menu() {
    createFileMenu();
    createEditMenu();
    createHelpMenu();
    addMenu(m_fileMenu);
    addMenu(m_editMenu);
    addMenu(m_helpMenu);
}

void Menu::hideMenuBar()
{
    setVisible(false);
}

void Menu::showMenuBar()
{
    setVisible(true);
}


void Menu::createFileMenu() {
    m_openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                               tr("&Open"), this);
    connect(m_openAction, &QAction::triggered, App::instance(), &App::openEditor);
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
    m_copyAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy),
                               tr("&Copy"), this);
    m_cutAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCut),
                              tr("&Cut"), this);
    m_pasteAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditPaste),
                                tr("&Paste"), this);
    m_modifyAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties),
                                tr("&Modify"), this);
    m_selectAllAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditSelectAll),
                                tr("&Select All"), this);
    m_clearSelectionAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditClear),
                                    tr("&Clear Selection"), this);
    m_deleteAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete),
                                    tr("&Delete"), this);
    m_editMenu = new QMenu(tr("&Edit"));
    m_editMenu->addActions({m_undoAction,
                            m_redoAction,
                            m_copyAction,
                            m_cutAction,
                            m_pasteAction,
                            m_modifyAction,
                            m_selectAllAction,
                            m_clearSelectionAction,
                            m_deleteAction});
}

void Menu::createHelpMenu() {
    m_keyMapAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::InputKeyboard),
                                     tr("&Key Map"), this);
    m_aboutAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
                                tr("&About"), this);
    m_helpMenu = new QMenu(tr("&Help"));
    m_helpMenu->addActions({m_keyMapAction, m_aboutAction});
}
