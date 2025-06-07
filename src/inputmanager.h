#pragma once

#include <QSet>
#include <QTimer>
#include <QKeyEvent>
#include <QObject>

class InputManager: public QObject {
public:
    explicit InputManager(QObject* parent = nullptr);

    void keyPressed(QKeyEvent* event);

    void keyReleased(QKeyEvent* event);

    bool isKeyPressed(int key) const;

signals:
    void inputUpdated(const QSet<int>& keys);

private slots:
    void processInput() {
        emit inputUpdated(m_pressedKeys);
    }

private:
    QTimer* m_inputTimer;
    QSet<int> m_pressedKeys;
};
