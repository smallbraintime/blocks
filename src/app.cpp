#include "app.h"

#include <QWidget>
#include <QApplication>

#include "style.h"

App::App() {
    QVector<std::pair<QString, std::function<void()>>> buttonCallbacks = {{"Load", [](){}}, {"New", [](){}}, {"Exit", [](){ QApplication::quit(); }}};
    mMenu = new Menu("BLOCKS", buttonCallbacks);
    mMenu->loadStyleSheet(STYLESHEET);
    mWindow.setWindowTitle("blocks");
    mWindow.setGeometry(0, 0, mWindow.maximumWidth(), mWindow.maximumHeight());
    mWindow.setCentralWidget(mMenu);
    mWindow.show();
}
