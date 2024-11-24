#include "menu.h"

Menu::Menu(const QString& title, const QVector<QPair<const char*, std::function<void(App&)>>>& buttonCallbacks, App& app) {
    mLayout = new QVBoxLayout(this);
    mLayout->setAlignment(Qt::AlignCenter);

    mTitle = new QLabel(title, this);
    mTitle->setAlignment(Qt::AlignCenter);
    mLayout->addWidget(mTitle, 0, Qt::AlignCenter);

    for (const auto& button : buttonCallbacks) {
        auto newButton = new QPushButton(button.first, this);
        mOptions.emplaceBack(newButton);
        connect(newButton, &QPushButton::clicked, this, [callback = button.second, &app]() {
            callback(app);
        });
        mLayout->addWidget(newButton);
    }
    setLayout(mLayout);
}

Menu::~Menu() {
    delete mTitle;
    for (const auto& button : mOptions) delete button;
    delete mLayout;
}
