#include "enginewidget.h"

#include <QDateTime>

EngineWidget::EngineWidget(QWidget* parent, float targetFps) : QWidget(parent), m_targetFps(targetFps)  {
    connect(m_timer, &QTimer::timeout, this, [=](){
        m_scene->update(m_deltaTime);

        qint64 currentFrame = QDateTime::currentMSecsSinceEpoch();
        m_deltaTime = (currentFrame - m_lastFrame) / 1000.0f;
        m_lastFrame = currentFrame;
    });
    m_timer->start(int(1000.0f / m_targetFps));
}

void EngineWidget::mouseMoveEvent(QMouseEvent *event) {
    m_scene->mouseMoveEvent(event);
}

void EngineWidget::keyPressEvent(QKeyEvent *event) {
    m_scene->keyPressEvent(event);
}

void EngineWidget::keyReleaseEvent(QKeyEvent *event) {
    m_scene->keyReleaseEvent(event);
}
