#include "editor.h"

#include <QApplication>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QSharedPointer>
#include <QVBoxLayout>

#include "app.h"

Editor::Editor(QWidget* parent) : QWidget(parent), m_camera{}, m_cameraController{&m_camera, .05f, 0.05f}, m_renderer{new BlocksRenderer(this, &m_camera, &m_pointedBlock)} {
    m_inputManager = new InputManager(this);
    connect(m_inputManager, &InputManager::inputUpdated, this, &Editor::onInputUpdate);

    // m_blocks.fill(QColor(255, 255, 255, 255), CHUNK_SIZE);
    // m_renderer->setBuffer(m_blocks);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_renderer);

    setFocusPolicy(Qt::StrongFocus);
    cursor().setPos(mapToGlobal(rect().center()));
    setMouseTracking(true);
    //setAttribute(Qt::WA_InputMethodEnabled, false);
}

void Editor::keyPressEvent(QKeyEvent *event) {
    m_inputManager->keyPressed(event);
}

void Editor::keyReleaseEvent(QKeyEvent *event) {
    m_inputManager->keyReleased(event);
}

void Editor::onInputUpdate(const QSet<int> &keys) {
    float delta = m_deltaTime;
    if (keys.contains(Qt::Key_Shift)) delta *= 2;
    if (keys.contains(Qt::Key_Space)) m_cameraController.moveUp(delta);
    if (keys.contains(Qt::Key_W)) m_cameraController.moveForward(-delta);
    if (keys.contains(Qt::Key_S)) m_cameraController.moveForward(delta);
    if (keys.contains(Qt::Key_A)) m_cameraController.moveRight(-delta);
    if (keys.contains(Qt::Key_D)) m_cameraController.moveRight(delta);
    if (keys.contains(Qt::Key_Escape)) {
        App::instance()->setOverrideCursor( QCursor( Qt::ArrowCursor ) );
        clearFocus();
    }
}

void Editor::mouseMoveEvent(QMouseEvent *event) {
    if (hasFocus()) {
        float xPos = event->globalPosition().x();
        float yPos = event->globalPosition().y();

        cursor().setPos(mapToGlobal(rect().center()));
        m_lastx = mapToGlobal(rect().center()).x();
        m_lasty = mapToGlobal(rect().center()).y();

        if (m_firstMouse) {
            m_lastx = xPos;
            m_lasty = yPos;
            m_firstMouse = false;
            return;
        }

        float xoffset = m_lastx - xPos;
        float yoffset = m_lasty - yPos;
        m_lastx = xPos;
        m_lasty = yPos;

        m_cameraController.yaw(xoffset);
        m_cameraController.pitch(yoffset);
    }
}

void Editor::mousePressEvent(QMouseEvent *event) {
    App::instance()->setOverrideCursor( QCursor( Qt::BlankCursor ) );
    setFocus();
}

void Editor::wheelEvent(QWheelEvent *event) {
    m_camera.setFov(m_camera.fov() - event->angleDelta().y() / 120);

    if (m_camera.fov() < 1.0f)
        m_camera.setFov(1.0f);
    if (m_camera.fov() > 120.0f)
        m_camera.setFov(120.0f);
}
