#include "menu.h"

#include <QCoreApplication>
#include <QMessageBox>

Menu::Menu(Editor* editor, QWidget* parent) : QMenuBar(parent) {
    createFileMenu();
    createToolsMenu();
    createHelpMenu();
    addMenu(m_fileMenu);
    addMenu(m_toolsMenu);
    addMenu(m_helpMenu);

    connect(m_changeColorAction, &QAction::triggered, editor, &Editor::setColor);
    connect(m_openAction, &QAction::triggered, editor, [=](){
        emit newTriggered();
        editor->openProject();
    });
    connect(m_saveAction, &QAction::triggered, editor, &Editor::saveProject);
}

void Menu::createFileMenu() {
    m_openAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                               tr("&Open"), this);
    m_newAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                              tr("&New"), this);
    m_saveAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                               tr("&Save"), this);
    m_exitAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit),
                               tr("&Exit"), this);
    m_fileMenu = new QMenu(tr("&File"));

    m_fileMenu->addActions({m_openAction, m_newAction, m_saveAction, m_exitAction});

    connect(m_newAction, &QAction::triggered, this, &Menu::newTriggered);
    connect(m_exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Menu::createToolsMenu() {
    m_changeColorAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties),
                                 tr("&Change Color"), this);
    m_toolsMenu = new QMenu(tr("&Tools"));

    m_toolsMenu->addActions({m_changeColorAction});
}

void Menu::createHelpMenu() {
    m_keyMapAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::InputKeyboard),
                                     tr("&Key Map"), this);
    m_aboutAction = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
                                tr("&About"), this);
    m_helpMenu = new QMenu(tr("&Help"));

    m_helpMenu->addActions({m_keyMapAction, m_aboutAction});

    connect(m_keyMapAction, &QAction::triggered, this, [](){
        QMessageBox::information(nullptr, "Key Mapping", "MMB - move camera\nLMB - place a block\nRMB + LMB - delete a block");
    });
    connect(m_aboutAction, &QAction::triggered, this, [](){
        QMessageBox::information(nullptr, "About", "BLOCKS 2025\na simple voxel editor");
    });
}
