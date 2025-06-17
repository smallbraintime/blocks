#include "renderer.h"

#include <QDateTime>
#include <QRandomGenerator>

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

    constexpr float verticies[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };
    constexpr int verticiesSize = sizeof(verticies) / sizeof(float);
    if (!m_renderContext.vbo.create()) {
        qWarning("Failed to create vbo");
    }
    m_renderContext.vbo.bind();
    m_renderContext.vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_renderContext.vbo.allocate(verticies, verticiesSize * sizeof(float));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    constexpr unsigned int indicies[] = {
        0, 1, 2,  2, 3, 0,
        4, 5, 6,  6, 7, 4,
        4, 5, 1,  1, 0, 4,
        7, 6, 2,  2, 3, 7,
        4, 0, 3,  3, 7, 4,
        5, 1, 2,  2, 6, 5
    };
    constexpr int indiciesSize = sizeof(indicies) / sizeof(unsigned int);
    if (!m_renderContext.ebo.create()) {
        qWarning("Failed to create ebo");
    }
    m_renderContext.ebo.bind();
    m_renderContext.ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_renderContext.ebo.allocate(indicies, indiciesSize * sizeof(unsigned int));

    m_renderPasses[0] = std::make_unique<BasePass>();
    // m_renderPasses[1] = std::make_unique<FXAAPass>();

    m_renderContext.vao.release();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    if (!m_renderContext.ssbo.create()) {
        qWarning("Failed to create ssbo");
    }

    struct Color {
        int r, g, b, a;
    };

    QVector<Color> blocks;
    for (size_t i = 0; i < 10000; ++i) {
        blocks.append(Color{QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255), QRandomGenerator::global()->bounded(255)});
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderContext.ssbo.bufferId());
    glBufferData(GL_SHADER_STORAGE_BUFFER, blocks.size() * sizeof(Color), blocks.constData(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_renderContext.ssbo.bufferId());
}

void BlocksRenderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_renderContext.camera->setAspectRatio((float)w / (float)h);
}

void BlocksRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_renderPasses[0]) {
        m_renderPasses[0]->render(m_renderContext);
    }
    // for (auto& pass : m_renderPasses) {
    //     if (pass) pass->render(m_renderContext);
    // }

    update();
}



