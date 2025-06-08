#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>

class RenderContext;

struct RenderPass: protected QOpenGLFunctions_4_3_Core {
    virtual ~RenderPass() {}
    virtual void render(RenderContext& sceneData) = 0;
};

class BasePass : public RenderPass {
public:
    BasePass();

    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    struct UniformLocations {
        int view;
        int projection;
    } m_uniformLocations;
};

class FXAAPass : public RenderPass {
public:
    FXAAPass();

    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
};
