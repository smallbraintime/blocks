#include "renderer.h"

#include <QDateTime>
#include <QRandomGenerator>
#include <QImage>

#include "data.h"

BlocksRenderer::BlocksRenderer(QWidget* parent, Camera* camera, QVector3D* pointedBlock) : QOpenGLWidget(parent) {
    m_renderContext.funcs = this;
    m_renderContext.camera = camera;
    m_renderContext.pointedBlock = pointedBlock;
    setMouseTracking(true);
}

BlocksRenderer::~BlocksRenderer() {
    delete m_renderContext.normalMap;
    delete m_renderContext.depthMap;
    delete m_renderContext.fbo;
}

void BlocksRenderer::setBuffer(const QVector<QColor> &blocks) {
    m_renderContext.ssbo.bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, blocks.size() * sizeof(QColor), blocks.constData(), GL_DYNAMIC_DRAW);
    m_renderContext.ssbo.release();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_renderContext.ssbo.bufferId());
}

void BlocksRenderer::initializeGL() {
    initializeOpenGLFunctions();

    if (!m_renderContext.vao.create()) {
        qWarning("Failed to create vao");
    }
    m_renderContext.vao.bind();

    if (!m_renderContext.vbo.create()) {
        qWarning("Failed to create vbo");
    }
    m_renderContext.vbo.bind();
    m_renderContext.vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_renderContext.vbo.allocate(CUBE_VERTICES, CUBE_VERTICES_SIZE * sizeof(float));

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
    glCullFace(GL_BACK);
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

    m_renderContext.normalMap = new QOpenGLTexture(QImage(":/assets/normal.png"));
    m_renderContext.normalMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_renderContext.normalMap->setMagnificationFilter(QOpenGLTexture::Linear);
    if (!m_renderContext.normalMap->isCreated()) {
        qWarning() << "Normal map not created!";
    }
    m_renderContext.normalMap->generateMipMaps();

    m_renderContext.texture = new QOpenGLTexture(QImage(":/assets/texture.png"));
    m_renderContext.texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_renderContext.texture->setMagnificationFilter(QOpenGLTexture::Linear);
    if (!m_renderContext.texture->isCreated()) {
        qWarning() << "Texture not created!";
    }
    m_renderContext.texture->generateMipMaps();

    m_renderContext.cubeMap = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    m_renderContext.cubeMap->create();
    m_renderContext.cubeMap->bind();
    m_renderContext.cubeMap->setSize(2048, 2048);
    m_renderContext.cubeMap->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    m_renderContext.cubeMap->setWrapMode(QOpenGLTexture::ClampToEdge);
    m_renderContext.cubeMap->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_renderContext.cubeMap->allocateStorage();
    QImage right(":/assets/right.jpg");
    right = right.convertToFormat(QImage::Format_RGBA8888);
    m_renderContext.cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, right.constBits());
    QImage top(":/assets/top.jpg");
    top = top.convertToFormat(QImage::Format_RGBA8888);
    m_renderContext.cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, top.constBits());
    QImage front(":/assets/front.jpg");
    front = front.convertToFormat(QImage::Format_RGBA8888);
    m_renderContext.cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, front.constBits());
    QImage left(":/assets/left.jpg");
    left = left.convertToFormat(QImage::Format_RGBA8888);
    m_renderContext.cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, left.constBits());
    QImage bottom(":/assets/bottom.jpg");
    bottom = bottom.convertToFormat(QImage::Format_RGBA8888);
    m_renderContext.cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, bottom.constBits());
    QImage back(":/assets/back.jpg");
    back = back.convertToFormat(QImage::Format_RGBA8888);
    m_renderContext.cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, back.constBits());
    if (!m_renderContext.cubeMap->isCreated()) {
        qWarning() << "Cubemap not created!";
    }
    m_renderContext.cubeMap->generateMipMaps();
    m_renderContext.cubeMap->release();

    m_renderContext.depthMap = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_renderContext.depthMap->create();
    m_renderContext.depthMap->bind();
    m_renderContext.depthMap->setFormat(QOpenGLTexture::D32F);
    m_renderContext.depthMap->setSize(SHADOW_WIDTH, SHADOW_HEIGHT);
    m_renderContext.depthMap->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    m_renderContext.depthMap->setWrapMode(QOpenGLTexture::ClampToEdge);
    m_renderContext.depthMap->allocateStorage();
    m_renderContext.depthMap->release();

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
    m_renderContext.fbo = new QOpenGLFramebufferObject(QSize(SHADOW_WIDTH, SHADOW_HEIGHT), format);

    GLuint fboId = m_renderContext.fbo->handle();
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_renderContext.depthMap->textureId(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        qWarning("Framebuffer not complete!");
    }

    m_renderContext.light = std::make_unique<Camera>(Camera::ProjectionMode::Orthogonal);
    m_renderContext.light->setAspectRatio(1.0f);
    m_renderContext.light->setNearPlane(10.0f);
    m_renderContext.light->setFarPlane(40.0f);
    m_renderContext.light->setFov(30.0f);
    m_renderContext.light->setPosition({30, 30.0f, 0});
    m_renderContext.light->lookAt({5.0f, 5.0f, 0.0f});
    // m_renderContext.light->setOrientation({-63.95, -0, 0});
    // m_renderContext.light->setOrientation({0, -90.0f, 0});

    m_renderPasses.push_back(std::make_unique<ShadowMapPass>());
    m_renderPasses.push_back(std::make_unique<BasePass>());
    m_renderPasses.push_back(std::make_unique<LightPosPass>());
    m_renderPasses.push_back(std::make_unique<BackgroundPass>());

    for (const auto& pass : m_renderPasses) {
        if (pass) pass->init(m_renderContext.funcs);
    }
}

void BlocksRenderer::resizeGL(int w, int h) {
    m_renderContext.m_screenWidth = w;
    m_renderContext.m_screenHeight = h;

    glViewport(0, 0, w, h);
    m_renderContext.camera->setAspectRatio((float)w / (float)h);
}

void BlocksRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& pass : m_renderPasses) {
        if (pass) pass->render(m_renderContext);
    }

    static float time = 0.0f;
    time = time + 0.016f;
    float z = sin(time * 0.1) * 30.0f;
    float x = cos(time * 0.1) * 30.0f;
    m_renderContext.light->setPosition({5.0f + x, 30.0f, 5.0f + z});
    m_renderContext.light->lookAt({5.0f, 5.0f, 5.0f});

    update();
}



