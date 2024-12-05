#include "editor.h"

#include <QApplication>
#include <QDateTime>
#include <QRandomGenerator>
#include <QImage>
#include <QImageReader>
#include <QMatrix4x4>

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

    float vertices[] = {
        -1.0f,  1.0f, 0.99f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.99f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, -1.0f, 0.99f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.99f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

        0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,    1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 1.0f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f
    };



    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,

        4, 5, 7,
        5, 6, 7,

        8, 9, 11,
        9, 10, 11,

        12, 13, 15,
        13, 14, 15,

        16, 17, 19,
        17, 18, 19,

        20, 21, 23,
        21, 22, 23,

        24, 25, 27,
        25, 26, 27
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

    QImage image;
    QImageReader ir(":/assets/texture.jpg");
    if (!ir.read(&image)) qDebug() << "Cant read the image";
    image = image.convertToFormat(QImage::Format_RGBA8888);

    m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    m_texture->create();
    m_texture->setSize(image.width(), image.height(), 1);
    m_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_texture->allocateStorage();
    m_texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, image.bits());
    m_texture->generateMipMaps(GL_TEXTURE_2D);
    if (m_texture->isCreated()) {
        m_texture->bind();
    } else {
        qDebug() << "Texture not created!";
    }
    m_texture->bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_vao.create();
    m_vao.bind();
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(float));
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 8 * sizeof(float));
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 2, 8 * sizeof(float));
    m_program->enableAttributeArray(2);

    glEnable(GL_DEPTH_TEST);

    for (size_t i = 0; i < 10; ++i) {
        QRandomGenerator generator;
        generator = generator.securelySeeded();
        const auto x = float(generator.generateDouble() * 2.0f - 1.0f);
        const auto y = float(generator.generateDouble() * 2.0f - 1.0f);
        positions.emplace_back(x, y, -3);
    }
}

void Editor::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Editor::paintGL() {
    repaint();
}

void Editor::repaint() {
    static int counter = 0;
    static float degree = 1;
    static float y = 0;
    static float direction = 0.01;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_ebo.bind();
    m_vao.bind();

    if (counter % 16 == 0) {
        m_program->setUniformValue("mulx", float(QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f));
        m_program->setUniformValue("muly", float(QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f));
        m_program->setUniformValue("mulz", float(QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f));
    }
    m_program->setUniformValue("isTextured", false);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    for (const auto& pos : positions) {
        QMatrix4x4 model;
        model.rotate(-45, 1, 0, 0);
        QMatrix4x4 view;
        view.translate(pos);
        QMatrix4x4 projection;
        projection.perspective(45, width() / height(), 0.1, 100);
        m_program->setUniformValue("model", model);
        m_program->setUniformValue("view", view);
        m_program->setUniformValue("projection", projection);
        QMatrix4x4 trans;
        trans.scale(sin(y));
        trans.translate(cos(y), sin(y), 0);
        trans.rotate(degree, cos(y), sin(y), sin(y));
        m_program->setUniformValue("isTextured", true);
        m_program->setUniformValue("trans", trans);
        m_texture->bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(6 * sizeof(int)));
    }

    m_texture->release();
    m_program->setUniformValue("trans", QMatrix4x4());
    m_program->setUniformValue("model", QMatrix4x4());
    m_program->setUniformValue("view", QMatrix4x4());
    m_program->setUniformValue("projection", QMatrix4x4());

    ++counter;
    ++degree;
    y += direction;
    direction = y == 1 ? -0.01 : 0.01;
    update();
}
