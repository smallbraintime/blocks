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
#include <QElapsedTimer>
#include <memory>
#include <vector>

#include "renderpass.h"
#include "camera.h"
#include "data.h"

struct RenderContext {
    QOpenGLFunctions_4_3_Core* funcs;
    Camera* camera{};
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer ssbo;
    std::unique_ptr<Camera> light;
    std::unique_ptr<QOpenGLTexture> normalMap;
    std::unique_ptr<QOpenGLTexture> texture;
    std::unique_ptr<QOpenGLFramebufferObject> fbo;
    std::unique_ptr<QOpenGLTexture> depthMap;
    std::unique_ptr<QOpenGLTexture> cubeMap;
    int* pointedBlock{};
    int screenWidth{};
    int screenHeight{};
};

class BlocksRenderer: public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core {
    Q_OBJECT

public:
    explicit BlocksRenderer(QWidget* parent, Camera* camera, int* pointedBlock);

    void setBuffer(const QVector<Color>& blocks);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    std::vector<std::unique_ptr<RenderPass>> m_renderPasses;
    RenderContext m_renderContext;

    void initBuffers();
    void initTexture();
    void initNormalMap();
    void initShadowMap();
    void initCubeMap();
    void initLight();
    void updateLight();

signals:
    void glInitialized();
};
