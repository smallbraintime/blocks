#pragma once

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QVector>
#include <QVector3D>
#include <QKeyEvent>

#include "camera.h"
#include "cameracontroller.h"
#include "viewport.h"

class Editor : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Editor();
    ~Editor();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    QOpenGLShaderProgram* m_program;
    QOpenGLTexture* m_texture;
    Viewport m_viewport;
    Camera m_camera;
    CameraController m_cameraController;
    float m_deltaTime = 0;
    qint64 m_lastFrame = 0;
    float m_lastx;
    float m_lasty;
    bool m_firstMouse = true;

    void repaint();
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
};


