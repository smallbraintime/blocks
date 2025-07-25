#include "editor.h"

#include <QApplication>
#include <QKeyEvent>
#include <QVBoxLayout>

Editor::Editor(QWidget* parent, Menu* menu) : QWidget(parent), m_menu(menu), m_camera({20.0f, 20.0f, 20.0f}), m_cameraController{&m_camera, .05f, 0.05f, {5.0f, 5.0f, 5.0f}, 10.0f}, m_renderer{new BlocksRenderer(this, &m_camera, &m_pointedBlock)} {
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    m_renderer->setFormat(format);

    // m_blocks.fill(QColor(255, 255, 255, 255), CHUNK_SIZE);
    // m_renderer->setBuffer(m_blocks);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_renderer);

    setFocusPolicy(Qt::StrongFocus);
    cursor().setPos(mapToGlobal(rect().center()));
    setMouseTracking(true);
    setAttribute(Qt::WA_InputMethodEnabled, false);
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

        m_cameraController.addYaw(-xoffset);
        m_cameraController.addPitch(yoffset);
    }
}

void Editor::mousePressEvent(QMouseEvent *event) {
    QApplication::setOverrideCursor( QCursor( Qt::BlankCursor ) );
    setFocus();
}

void Editor::wheelEvent(QWheelEvent *event) {
    float delta = event->angleDelta().y() / 120.0f;
    m_cameraController.setRadius(m_cameraController.radius() - delta * 2.0f);
}
