#include "renderer.h"

BlocksRenderer::BlocksRenderer(QWidget* parent, Camera* camera, QVector3D* pointedBlock) : QOpenGLWidget(parent) {
    m_renderContext.camera = camera;
    m_renderContext.pointedBlock = pointedBlock;
}

void BlocksRenderer::setBuffer(const QVector<Color> &blocks) {
    glBufferData(GL_SHADER_STORAGE_BUFFER, blocks.size() * sizeof(Color), blocks.constData(), GL_DYNAMIC_DRAW);
}

void BlocksRenderer::initializeGL() {
    initializeOpenGLFunctions();

    if (!m_renderContext.vao.create()) {
        qWarning("Failed to create vao");
    }
    m_renderContext.vao.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    constexpr float verticies[] = {
        -1.0f,  1.0f, 0.99f,
        -1.0f, -1.0f, 0.99f,
        1.0f, -1.0f, 0.99f,
        1.0f,  1.0f, 0.99f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };
    constexpr int verticiesSize = 0;
    if (!m_renderContext.vbo.create()) {
        qWarning("Failed to create vbo");
    }
    m_renderContext.vbo.bind();
    m_renderContext.vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_renderContext.vbo.allocate(verticies, verticiesSize * sizeof(float));

    constexpr unsigned int indicies[] = {
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
    constexpr int indiciesSize = 0;
    if (!m_renderContext.ebo.create()) {
        qWarning("Failed to create ebo");
    }
    m_renderContext.vbo.bind();
    m_renderContext.vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_renderContext.vbo.allocate(verticies, verticiesSize * sizeof(unsigned int));

    if (!m_renderContext.ssbo.create()) {
        qWarning("Failed to create ssbo");
    }

    m_renderPasses[0] = std::make_unique<BasePass>();
    m_renderPasses[1] = std::make_unique<FXAAPass>();

    m_renderContext.vao.release();
}

void BlocksRenderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_renderContext.camera->setAspectRatio(w / h);
}

void BlocksRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& pass : m_renderPasses) {
        pass->render(m_renderContext);
    }

    update();
}



