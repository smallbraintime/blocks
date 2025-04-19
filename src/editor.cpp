#include "editor.h"

#include <QApplication>
#include <QDateTime>
#include <QRandomGenerator>
#include <QImage>
#include <QImageReader>
#include <QMatrix4x4>

#include "app.h"

Editor::Editor()
    : m_camera(), m_viewport(45.0f, (float)width() / (float)height(), 0.1f, 100.0f), m_cameraController(m_camera, 1, 0.1) {
    setFocusPolicy(Qt::StrongFocus);
    cursor().setPos(mapToGlobal(rect().center()));
    setMouseTracking(true);    
}

Editor::~Editor() {
    m_vao.destroy();
    m_vbo.destroy();
    delete m_program;
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
        qDebug() << "Shader linker errors :\n" <<  m_program->log();

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
}

void Editor::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_viewport.setAspectRatio(w / h);
}

void Editor::paintGL() {
    repaint();
}

void Editor::repaint() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_ebo.bind();
    m_vao.bind();

    //m_program->setUniformValue("mulx", float(QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f));
    //m_program->setUniformValue("muly", float(QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f));
    //m_program->setUniformValue("mulz", float(QRandomGenerator::global()->generateDouble() * (0.8f - 0.2f) + 0.2f));

    m_program->setUniformValue("isTextured", false);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    QMatrix4x4 model;
    model.translate({0.0f, 0.0f, 0.0f});
    QMatrix4x4 view = m_camera.view();
    QMatrix4x4 projection = m_viewport.projection();
    m_program->setUniformValue("model", model);
    m_program->setUniformValue("view", view);
    m_program->setUniformValue("projection", projection);
    m_program->setUniformValue("isTextured", true);
    m_texture->bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(6 * sizeof(int)));

    m_texture->release();
    m_program->setUniformValue("trans", QMatrix4x4());
    m_program->setUniformValue("model", QMatrix4x4());
    m_program->setUniformValue("view", QMatrix4x4());
    m_program->setUniformValue("projection", QMatrix4x4());

    if (hasFocus()) {
        cursor().setPos(mapToGlobal(rect().center()));
        m_lastx = mapToGlobal(rect().center()).x();
        m_lasty = mapToGlobal(rect().center()).y();
    }

    qint64 currentFrame = QDateTime::currentMSecsSinceEpoch();
    m_deltaTime = (currentFrame - m_lastFrame) / 1000.0f;
    m_lastFrame = currentFrame;

    update();
}

void Editor::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_W: {
        m_cameraController.moveForward(-m_deltaTime);
        break;
    }
    case Qt::Key_S: {
        m_cameraController.moveForward(m_deltaTime);
        break;
    }
    case Qt::Key_A: {
        m_cameraController.moveRight(-m_deltaTime);
        break;
    }
    case Qt::Key_D: {
         m_cameraController.moveRight(m_deltaTime);
        break;
    }
    case Qt::Key_Escape: {
        App::instance()->setOverrideCursor( QCursor( Qt::ArrowCursor ) );
        clearFocus();
        break;
    }
    }
}

void Editor::mouseMoveEvent(QMouseEvent *event) {
    if (hasFocus()) {
        float xPos = event->globalPosition().x();
        float yPos = event->globalPosition().y();

        if (m_firstMouse) {
            m_lastx = xPos;
            m_lasty = yPos;
            m_firstMouse = false;
            return;
        }

        float xoffset = m_lastx - xPos;
        float yoffset = m_lasty - yPos;
        m_lastx = xPos;
        m_lasty = yPos;

        m_cameraController.yaw(xoffset);
        m_cameraController.pitch(yoffset);
        //m_cameraController.applyMouseMovement(xoffset, yoffset);
    }
}

void Editor::mousePressEvent(QMouseEvent *event) {
    App::instance()->setOverrideCursor( QCursor( Qt::BlankCursor ) );
    setFocus();
}

void Editor::wheelEvent(QWheelEvent *event) {
    m_viewport.setFov(m_viewport.fov() - event->angleDelta().y() / 120);

    if (m_viewport.fov() < 1.0f)
        m_viewport.setFov(1.0f);
    if (m_viewport.fov() > 120.0f)
        m_viewport.setFov(120.0f);
}
