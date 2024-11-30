#include "editor.h"

#include <QApplication>
#include <QDateTime>
#include <QRandomGenerator>
#include <QThread>

Editor::Editor() {
}

Editor::~Editor() {
    makeCurrent();
    m_vao.destroy();
    m_vbo.destroy();
    delete m_program;
    doneCurrent();
}

void Editor::initializeGL() {
    initializeOpenGLFunctions();

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


    float vertices2[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo.create();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices) );

    m_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo.create();
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ebo.bind();
    m_ebo.allocate(indices, sizeof(indices));
    m_ebo.bind();

    m_vao.create();
    m_vao.bind();
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    m_program->enableAttributeArray(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Editor::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Editor::paintGL() {
    repaint();
}

void Editor::repaint() {
    static int counter = 0;
    if (counter % 16 == 0) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_program->bind();

        float multiplier = (sin(QDateTime::currentDateTime().time().msec()) / 2.0f) + 0.5f;
        if (const int uniLocation = m_program->uniformLocation("mulx"); uniLocation == -1)
            qDebug() << "Cannot find uniform var";
        else
            m_program->setUniformValue(uniLocation, multiplier);
        float multiplier2 = QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f;
        float multiplier3 = QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f;
        m_program->setUniformValue("muly", multiplier2);
        m_program->setUniformValue("mulz", multiplier3);

        m_ebo.bind();
        m_vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    ++counter;
    update();
}
