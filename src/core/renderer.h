#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "scene.h"

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Renderer(QStringView vertexShaderPath, QStringView fragmentShaderPath);

    void render(const Scene& scene);

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLShaderProgram m_shaderProgram;
};
