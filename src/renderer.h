#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QSharedPointer>
#include <QOpenGLBuffer>
#include <QWidget>
#include <QVector3D>
#include <QColor>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFramebufferObject>
#include <memory>

#include "renderpass.h"
#include "camera.h"

struct RenderContext {
    QOpenGLFunctions_4_3_Core* funcs;
    Camera* camera;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer ssbo;
    QOpenGLTexture* normalMap;
    QVector3D* pointedBlock;
};

class BlocksRenderer: public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core {
    Q_OBJECT

public:
    explicit BlocksRenderer(QWidget* parent, Camera* camera, QVector3D* pointedBlock);
    ~BlocksRenderer();

    void setBuffer(const QVector<QColor>& blocks);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    std::unique_ptr<RenderPass> m_renderPasses[2];
    RenderContext m_renderContext;
};
