#pragma once

#include <QWidget>
#include <QColor>

#include "renderer.h"
#include "cameracontroller.h"
#include "inputmanager.h"
#include "menu.h"

#define CHUNK_SIZE 1000000

class Editor: public QWidget {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr, Menu* menu = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    BlocksRenderer* m_renderer;
    InputManager* m_inputManager;
    Menu* m_menu;
    CameraController m_cameraController;
    QVector<QColor> m_blocks{CHUNK_SIZE};
    QVector3D m_pointedBlock;
    Camera m_camera;
    float m_deltaTime = 1.0;
    float m_firstMouse;
    float m_lastx;
    float m_lasty;

private slots:
    void onInputUpdate(const QSet<int>& keys);
};


