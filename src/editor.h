#pragma once

#include <QWidget>

#include "scene.h"
#include "renderer.h"
#include "cameracontroller.h"

class Editor: public QWidget {
    Q_OBJECT

public:
    Editor(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    BlocksRenderer* m_renderer;
    Scene m_scene;
    CameraController m_cameraController;
    float m_deltaTime;
    float m_firstMouse;
    float m_lastx;
    float m_lasty;
};


