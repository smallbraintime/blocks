#pragma once

#include <QOpenGLShaderProgram>

class RenderContext;

class RenderPass {
public:
    virtual void begin() = 0;
    virtual void render(const RenderContext& sceneData) = 0;
    virtual void end() = 0;
};

class ZPass : RenderPass {
public:
    ZPass();

    void begin() override;
    void render(const RenderContext& renderContext) override;
    void end() override;

private:
    QOpenGLShaderProgram m_shaderProgram;

    struct UniformLocations {
        int model;
        int view;
        int projection;
        int position;
    } m_uniformLocations;
};

class BasePass : RenderPass {
public:
    BasePass();

    void begin() override;
    void render(const RenderContext& renderContext) override;
    void end() override;

private:
    QOpenGLShaderProgram m_shaderProgram;

    struct UniformLocations {
        int model;
        int view;
        int projection;
        int position;
        int color;
    } m_uniformLocations;

};

class LightPass : RenderPass {
public:
    LightPass();

    void begin() override;
    void render(const RenderContext& renderContext) override;
    void end() override;

private:
    QOpenGLShaderProgram m_shaderProgram;

    struct UniformLocations {
        int model;
        int view;
        int projection;
        int type;
        int color;
        int direction;
        int intensity;
        int position;
    } m_uniformLocations;
};

class PostprocessPass : RenderPass {};
