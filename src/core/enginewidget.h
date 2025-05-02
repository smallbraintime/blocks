#pragma once

#include <QWidget>
#include <QTimer>

#include "scene.h"
#include "renderer.h"

class EngineWidget : public QWidget {
    Q_OBJECT

public:
    explicit EngineWidget(QWidget* parent, float targetFps);

private:
    Scene* m_scene = new Scene(this);
    Renderer* m_renderer = new Renderer(this, m_scene->getSceneData(), u"", u"");
    QTimer* m_timer = new QTimer(this);
    float m_deltaTime;
    qint64 m_lastFrame;
    float m_targetFps;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};
