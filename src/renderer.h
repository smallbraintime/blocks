#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QSharedPointer>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <memory>

#include "renderpass.h"
#include "camera.h"

struct RenderContext {
    QSharedPointer<Camera> camera;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer ebo{QOpenGLBuffer::IndexBuffer};
    QOpenGLBuffer ssbo;
};

class BlocksRenderer: public QOpenGLWidget, protected  QOpenGLFunctions {
    Q_OBJECT

public:
    BlocksRenderer(QOpenGLWidget* parent = nullptr, QSharedPointer<Camera> camera = {});

    void setBuffer(const QVector<QColor>& blocks);
    void render();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QVector<std::unique_ptr<RenderPass>> m_renderPasses;
    RenderContext m_renderContext;
};
