#include "inputmanager.h"

InputManager::InputManager(QObject *parent) : QObject(parent) {
    m_inputTimer = new QTimer(this);
    connect(m_inputTimer, &QTimer::timeout, this, &InputManager::processInput);
    m_inputTimer->start(16);
}

void InputManager::keyPressed(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        m_pressedKeys.insert(event->key());
    }
}

void InputManager::keyReleased(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        m_pressedKeys.remove(event->key());
    }
}

bool InputManager::isKeyPressed(int key) const {
    return m_pressedKeys.contains(key);
}
