#pragma once

#include <QOpenGLShaderProgram>

class GraphicsComponent {
public:
    GraphicsComponent() = default;
    virtual void beginFrame(QOpenGLShaderProgram& program);
    virtual void endFrame(QOpenGLShaderProgram& program);
    virtual ~GraphicsComponent() = default;

    GraphicsComponent(const GraphicsComponent&) = delete;
    GraphicsComponent& operator=(const GraphicsComponent&) = delete;

private:
    bool m_valid;
};
