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
    QVector<QVector3D> m_positions;
    QVector3D m_cameraPos;
    QVector3D m_cameraFront;
    QVector3D m_cameraUp;
    float m_deltaTime = 0;
    qint64 m_lastFrame = 0;
    float m_lastx;
    float m_lasty;
    float m_yaw;
    float m_pitch;
    bool m_firstMouse = true;
    float m_fov = 45;
    void repaint();
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
};


