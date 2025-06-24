#include "renderer.h"

#include <QDateTime>
#include <QRandomGenerator>
#include <QImage>
#include <QImageReader>

BlocksRenderer::BlocksRenderer(QWidget* parent, Camera* camera, QVector3D* pointedBlock) : QOpenGLWidget(parent) {
    m_renderContext.funcs = this;
    m_renderContext.camera = camera;
    m_renderContext.pointedBlock = pointedBlock;
    setMouseTracking(true);
}

void BlocksRenderer::setBuffer(const QVector<QColor> &blocks) {
    m_renderContext.ssbo.bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, blocks.size() * sizeof(QColor), blocks.constData(), GL_DYNAMIC_DRAW);
    m_renderContext.ssbo.release();
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_renderContext.ssbo.bufferId());
}

void BlocksRenderer::initializeGL() {
    initializeOpenGLFunctions();
    if (!m_renderContext.vao.create()) {
        qWarning("Failed to create vao");
    }
    m_renderContext.vao.bind();

    constexpr float vertices[] = {
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,

        0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   -1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,   -1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,   -1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,   -1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f,   -1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,   -1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,     0.0f, 0.0f, -1.0f,    0.0f, 1.0f, 0.0f,

        0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,

        0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

        0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,

        0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f
    };

    constexpr int verticiesSize = sizeof(vertices) / sizeof(float);
    if (!m_renderContext.vbo.create()) {
        qWarning("Failed to create vbo");
    }
    m_renderContext.vbo.bind();
    m_renderContext.vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_renderContext.vbo.allocate(vertices, verticiesSize * sizeof(float));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);

    m_renderContext.vao.release();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    if (!m_renderContext.ssbo.create()) {
        qWarning("Failed to create ssbo");
    }

    struct Color {
        int r, g, b, a;
    };

    QVector<Color> blocks;
    for (size_t i = 0; i < 1000; ++i) {
        blocks.append(Color{QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(2) * 255});
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderContext.ssbo.bufferId());
    glBufferData(GL_SHADER_STORAGE_BUFFER, blocks.size() * sizeof(Color), blocks.constData(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_renderContext.ssbo.bufferId());

    m_renderPasses[0] = std::make_unique<BackgroundPass>();
    m_renderPasses[1] = std::make_unique<BasePass>();

    for (auto& pass : m_renderPasses) {
        if (pass) pass->init(m_renderContext.funcs);
    }

    QImage normalMap;
    QImageReader ir(":/assets/normal.png");
    if (!ir.read(&normalMap)) qWarning() << "Cant read the image";
    normalMap = normalMap.convertToFormat(QImage::Format_RGB888);

    m_renderContext.normalMap = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_renderContext.normalMap->create();
    m_renderContext.normalMap->setSize(normalMap.width(), normalMap.height(), 1);
    m_renderContext.normalMap->setFormat(QOpenGLTexture::RGB8_UNorm);
    m_renderContext.normalMap->allocateStorage();
    m_renderContext.normalMap->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, normalMap.bits());
    if (!m_renderContext.normalMap->isCreated()) {
        qWarning() << "Normal map not created!";
    }
}

void BlocksRenderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_renderContext.camera->setAspectRatio((float)w / (float)h);
}

void BlocksRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& pass : m_renderPasses) {
        if (pass) pass->render(m_renderContext);
    }

    update();
}



