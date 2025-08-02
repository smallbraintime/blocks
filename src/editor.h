#pragma once

#include <QWidget>
#include <QColor>
#include <QVBoxLayout>

#include "renderer.h"
#include "cameracontroller.h"
#include "data.h"

class Editor: public QWidget {
    Q_OBJECT

    enum class EditorMode {
        Creating,
        Deleting
    };

public:
    explicit Editor(QWidget* parent = nullptr);
    void setColor();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Camera m_camera;
    CameraController m_cameraController;
    BlocksRenderer* m_renderer;
    QVector<Color> m_blocks;
    QColor m_currentColor;
    int m_pointedBlock = -1;
    EditorMode m_editorMode = EditorMode::Creating;
    bool m_shouldMove{};
    float m_firstMouse{};
    float m_lastx{};
    float m_lasty{};

    void onGlInitialized();
    bool pointBlock(QMouseEvent *event);
};


