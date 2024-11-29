#include "editor.h"

#include <QApplication>

Editor::Editor() {}

void Editor::initializeGL() {
    m_program = new QOpenGLShaderProgram();
    if (!m_program->addShaderFromSourceFile(
            QOpenGLShader::Vertex, ":/shaders/basic.vert"))
    {
        qDebug() << "Vertex shader errors :\n" << m_program->log();
    }
    if (!m_program->addShaderFromSourceFile(
            QOpenGLShader::Fragment, ":/shaders/basic.frag"))
    {
        qDebug() << "Fragment shader errors :\n" << m_program->log();
    }
    if (!m_program->link())
        qDebug() << "Shader linker errors :\n" << m_program->log();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBufferObject.create();
    m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBufferObject.bind();
    m_vertexBufferObject.allocate(vertices, sizeof(vertices) );

    m_vao.create();
    m_vao.bind();
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    m_vertexBufferObject.release();
    m_vao.release();
}

void Editor::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Editor::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->bind();
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_vao.release();
}
