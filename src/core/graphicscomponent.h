#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class GraphicsComponent {
public:
    GraphicsComponent() = default;
    virtual void init(QOpenGLVertexArrayObject& vao);
    virtual void release();
    virtual void beginFrame(QOpenGLShaderProgram& program);
    virtual void endFrame(QOpenGLShaderProgram& program);
    virtual ~GraphicsComponent() = default;

    GraphicsComponent(const GraphicsComponent&) = delete;
    GraphicsComponent& operator=(const GraphicsComponent&) = delete;
};
