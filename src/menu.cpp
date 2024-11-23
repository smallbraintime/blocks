#include "menu.h"

Menu::Menu(const QString& title, const QVector<QPair<QString, std::function<void()>>>& buttonCallbacks) {
    mLayout = new QVBoxLayout(this);
    mLayout->setAlignment(Qt::AlignCenter);
    mTitle = new QLabel(title, this);
    mTitle->setAlignment(Qt::AlignCenter);
    mLayout->addWidget(mTitle, 0, Qt::AlignCenter);
    for (const auto& button : buttonCallbacks) {
        mOptions.emplaceBack(new QPushButton(button.first, this));
        connect(mOptions.back(), &QPushButton::clicked, this, button.second);
        mLayout->addWidget(mOptions.back());
    }
    setLayout(mLayout);
}

void Menu::loadStyleSheet(const QString& styleSheet) {
    setStyleSheet(styleSheet);
}
