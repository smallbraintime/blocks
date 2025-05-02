#pragma once

#include <QOpenGLShaderProgram>

class GraphicsComponent {
public:
    virtual void beginFrame(QOpenGLShaderProgram& program);
    virtual void endFrame(QOpenGLShaderProgram& program);
    virtual ~GraphicsComponent() = default;

    GraphicsComponent(const GraphicsComponent&) = delete;
    GraphicsComponent& operator=(const GraphicsComponent&) = delete;

private:
    QStringView m_id;
    bool m_valid;

    friend class Entity;
};
