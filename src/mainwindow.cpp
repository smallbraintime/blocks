#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) {
    m_editor = new Editor(this);
    m_menu = new Menu(m_editor, this);
    m_startPage = new QLabel("BLOCKS");
    m_startPage->setAlignment(Qt::AlignCenter);

    m_mainWidgets = new QStackedWidget();
    m_mainWidgets->addWidget(m_startPage);
    m_mainWidgets->addWidget(m_editor);
    m_mainWidgets->setCurrentWidget(m_startPage);

    setMenuBar(m_menu);
    setCentralWidget(m_mainWidgets);
    setWindowTitle("blocks");
    setGeometry(0, 0, maximumWidth(), maximumHeight());

    connect(m_menu, &Menu::newTriggered, this, &MainWindow::openEditor);
}

void MainWindow::openEditor() {
    m_mainWidgets->setCurrentWidget(m_editor);
}
