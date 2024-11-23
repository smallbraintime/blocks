#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class Menu : public QWidget
{
    Q_OBJECT

public:
    Menu(const QString& title, const QVector<QPair<QString, std::function<void()>>>& buttonCallbacks);
    void loadStyleSheet(const QString& styleSheet);

private:
    QLabel* mTitle;
    QVBoxLayout* mLayout;
    QVector<QPushButton*> mOptions;
};

#endif // MENU_H
