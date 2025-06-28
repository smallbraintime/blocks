#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLTexture>

class RenderContext;

struct RenderPass {
    virtual ~RenderPass() {}
    virtual void init(QOpenGLFunctions_4_3_Core* funcs) = 0;
    virtual void render(RenderContext& sceneData) = 0;
};

class BackgroundPass : public RenderPass {
public:
    void init(QOpenGLFunctions_4_3_Core* funcs) override;
    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    struct UniformLocations {
        int view;
        int projection;
    } m_uniformLocations;
};

class BasePass : public RenderPass {
public:
    void init(QOpenGLFunctions_4_3_Core* funcs) override;
    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    struct UniformLocations {
        int view;
        int projection;
        int viewPos;
    } m_uniformLocations;
};
