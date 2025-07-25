#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QColor>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <memory>
#include <vector>

#include "renderpass.h"
#include "camera.h"

struct RenderContext {
    QOpenGLFunctions_4_3_Core* funcs;
    Camera* camera;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer ssbo;
    QOpenGLTexture* normalMap;
    QOpenGLTexture* texture;
    QOpenGLFramebufferObject* fbo;
    QOpenGLTexture* depthMap;
    QOpenGLTexture* cubeMap;
    std::unique_ptr<Camera> light;
    QVector3D* pointedBlock;
    int m_screenWidth;
    int m_screenHeight;
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
    std::vector<std::unique_ptr<RenderPass>> m_renderPasses;
public:
    RenderContext m_renderContext;
};
