#include "app.h"

#include <QWidget>
#include <QApplication>
#include <QFileDialog>

#include "style.h"

App::App(int& argc, char** argv) : QApplication(argc, argv) {
    m_menu = new Menu();
    m_editor = new Editor();
    m_startPage = new QLabel("BLOCKS");
    m_startPage->setAlignment(Qt::AlignCenter);
    m_startPage->setStyleSheet(STYLESHEET);

    m_mainWidgets = new QStackedWidget();
    m_mainWidgets->addWidget(m_startPage);
    m_mainWidgets->addWidget(m_editor);
    m_mainWidgets->setCurrentWidget(m_startPage);

    m_window.setMenuBar(m_menu);
    m_window.setCentralWidget(m_mainWidgets);
    m_window.setWindowTitle("blocks");
    m_window.setGeometry(0, 0, m_window.maximumWidth(), m_window.maximumHeight());
    m_window.show();
}

App* App::instance() {
    static auto app = static_cast<App*>(QCoreApplication::instance());
    return app;
}

void App::openEditor() {
    m_mainWidgets->setCurrentWidget(m_editor);
}
