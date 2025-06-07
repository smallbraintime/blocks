#include "editor.h"

#include <QApplication>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QSharedPointer>

#include "app.h"

Editor::Editor(QWidget* parent) : QWidget(parent) {
    QSharedPointer<Camera> camera(new Camera());
    m_renderer = new BlocksRenderer(this, std::move(camera));

    setFocusPolicy(Qt::StrongFocus);
    cursor().setPos(mapToGlobal(rect().center()));
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled, false);
}

void Editor::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_W: {
        m_cameraController.moveForward(-m_deltaTime);
        break;
    }
    case Qt::Key_S: {
        m_cameraController.moveForward(m_deltaTime);
        break;
    }
    case Qt::Key_A: {
        m_cameraController.moveRight(-m_deltaTime);
        break;
    }
    case Qt::Key_D: {
         m_cameraController.moveRight(m_deltaTime);
        break;
    }
    case Qt::Key_Escape: {
        App::instance()->setOverrideCursor( QCursor( Qt::ArrowCursor ) );
        clearFocus();
        break;
    }
    }
}

void Editor::mouseMoveEvent(QMouseEvent *event) {
    if (hasFocus()) {
        float xPos = event->globalPosition().x();
        float yPos = event->globalPosition().y();

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
    // m_viewport.setFov(m_viewport.fov() - event->angleDelta().y() / 120);

    // if (m_viewport.fov() < 1.0f)
    //     m_viewport.setFov(1.0f);
    // if (m_viewport.fov() > 120.0f)
    //     m_viewport.setFov(120.0f);
}
