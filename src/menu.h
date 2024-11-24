#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "app.h"
class App;

class Menu : public QWidget
{
    Q_OBJECT

public:
    Menu(const QString& title, const QVector<QPair<const char*, std::function<void(App&)>>>& buttonCallbacks, App& app);
    ~Menu();

private:
    QLabel* mTitle;
    QVBoxLayout* mLayout;
    QVector<QPushButton*> mOptions;
};
