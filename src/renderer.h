#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QSharedPointer>
#include <QOpenGLBuffer>
#include <QWidget>
#include <QVector3D>
#include <memory>

#include "renderpass.h"
#include "camera.h"

struct RenderContext {
    Camera* camera;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer ebo{QOpenGLBuffer::IndexBuffer};
    QOpenGLBuffer ssbo;
    QVector3D* pointedBlock;
};

struct Color {
    float r, g, b, a;
};

class BlocksRenderer: public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core {
    Q_OBJECT

public:
    explicit BlocksRenderer(QWidget* parent, Camera* camera, QVector3D* pointedBlock);

    void setBuffer(const QVector<Color>& blocks);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    std::unique_ptr<RenderPass> m_renderPasses[2];
    RenderContext m_renderContext;
};
