#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "scene.h"

class RenderPass {
public:
    virtual void begin() = 0;
    virtual void render(const Scene::SceneData& sceneData) = 0;
    virtual void end() = 0;
};

class ZPass : RenderPass {
public:
    ZPass();

    void begin() override;
    void render(const Scene::SceneData& sceneData) override;
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
    void render(const Scene::SceneData& sceneData) override;
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
    void render(const Scene::SceneData& sceneData) override;
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

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Renderer(QWidget* parent, const Scene::SceneData& sceneData,  QStringView vertexShaderPath, QStringView fragmentShaderPath);

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void render();
    void setSceneData(const Scene::SceneData* sceneData) { m_sceneData = sceneData; }

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLShaderProgram m_zpassShader;
    QOpenGLShaderProgram m_baseShader;
    QOpenGLShaderProgram m_postprocessShader;
    const Scene::SceneData* m_sceneData;
};
