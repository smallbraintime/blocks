#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "scene.h"

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Renderer(QWidget* parent, const SceneData& sceneData,  QStringView vertexShaderPath, QStringView fragmentShaderPath);

    void render();
    void setSceneData(const SceneData& sceneData) { m_sceneData = sceneData; }

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLShaderProgram m_shaderProgram;
    SceneData& m_sceneData;
};
