#include "app.h"

#include <QWidget>
#include <QApplication>
#include <QFileDialog>

#include "style.h"

App::App() {
    const QVector<QPair<const char*, std::function<void(App&)>>> buttonCallbacks = {
        {"Load", [&](App& app){
             //const auto filePath = QFileDialog::getOpenFileName(this, "Choose file to load", QDir::homePath(), "XML Files (*.xml)");
         }},
        {"New", [&](App& app){
             //const auto dirPath = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath(), QFileDialog::ShowDirsOnly);
             mStackedWidget->setCurrentWidget(mGame);
        }},
        {"Exit", [&](App& app){ QApplication::quit(); }}
    };
    mMenu = new Menu("BLOCKS", buttonCallbacks, *this);
    mMenu->setStyleSheet(STYLESHEET);

    mGame = new Game();
    //connect(mGame, &Game::backToMenu, this, &App::onBackToMenu);

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->addWidget(mMenu);
    mStackedWidget->addWidget(mGame);

    mWindow.setWindowTitle("blocks");
    mWindow.setGeometry(0, 0, mWindow.maximumWidth(), mWindow.maximumHeight());
    mWindow.setCentralWidget(mStackedWidget);
    mWindow.show();
}

App::~App() {
    delete mGame;
    delete mMenu;
    delete mStackedWidget;
}

void App::onBackToMenu() {
    mStackedWidget->setCurrentWidget(mMenu);
}
