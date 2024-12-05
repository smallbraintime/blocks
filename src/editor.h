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
    QOpenGLContext m_context;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    QOpenGLShaderProgram* m_program;
    QOpenGLTexture* m_texture;
    QVector<QVector3D> positions;
    void repaint();
};
