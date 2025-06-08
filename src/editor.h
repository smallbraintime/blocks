#pragma once

#include <QWidget>

#include "renderer.h"
#include "cameracontroller.h"
#include "inputmanager.h"

#define CHUNK_SIZE 1000

class Editor: public QWidget {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    BlocksRenderer* m_renderer;
    InputManager* m_inputManager;
    CameraController m_cameraController;
    QVector<Color> m_blocks{CHUNK_SIZE};
    QVector3D m_pointedBlock;
    Camera m_camera;
    float m_deltaTime;
    float m_firstMouse;
    float m_lastx;
    float m_lasty;

private slots:
    void onInputUpdate(const QSet<int>& keys);
};


