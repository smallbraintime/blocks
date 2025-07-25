#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_3_Core>

class RenderContext;

struct RenderPass {
    virtual ~RenderPass() {}
    virtual void init(QOpenGLFunctions_4_3_Core* funcs) = 0;
    virtual void render(RenderContext& sceneData) = 0;
};

class LightPosPass : public RenderPass {
public:
    void init(QOpenGLFunctions_4_3_Core* funcs) override;
    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    struct Uniforms {
        int viewProj;
        int lightPos;
        int color;
    } m_uniforms;
};

class BackgroundPass : public RenderPass {
public:
    void init(QOpenGLFunctions_4_3_Core* funcs) override;
    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    int m_viewProjUniform;
};

class BasePass : public RenderPass {
public:
    void init(QOpenGLFunctions_4_3_Core* funcs) override;
    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_baseShaderProgram;
    QOpenGLShaderProgram m_wireframeShaderProgram;

    void renderBase(RenderContext& renderContext);
    void renderWireframe(RenderContext& renderContext);

    struct Uniforms {
        int viewProj;
        int cameraPos;
        int normalMap;
        int texture;
        int depthMap;
        int lightViewProj;
        int lightPos;
        int skybox;
        int color;
    } m_uniforms;
};

class ShadowMapPass : public RenderPass {
public:
    void init(QOpenGLFunctions_4_3_Core* funcs) override;
    void render(RenderContext& renderContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    int m_viewProjUniform;
};

class LightDepthPass : public RenderPass {
public:
    void init(QOpenGLFunctions_4_3_Core* funcs) override;
    void render(RenderContext& rednerContext) override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLVertexArrayObject m_vao;
};
