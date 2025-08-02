#include "renderer.h"

#include <QImage>
#include <QTimer>

#include "data.h"

BlocksRenderer::BlocksRenderer(QWidget* parent, Camera* camera, int* pointedBlock) : QOpenGLWidget(parent) {
    m_renderContext.funcs = this;
    m_renderContext.camera = camera;
    m_renderContext.pointedBlock = pointedBlock;

    setMouseTracking(true);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&QOpenGLWidget::update));
    timer->start(16);
}

void BlocksRenderer::initializeGL() {
    initializeOpenGLFunctions();

    initBuffers();
    initTexture();
    initNormalMap();
    initShadowMap();
    initCubeMap();
    initLight();

    m_renderPasses.push_back(std::make_unique<ShadowMapPass>());
    // m_renderPasses.push_back(std::make_unique<LightDepthPass>());
    m_renderPasses.push_back(std::make_unique<BasePass>());
    m_renderPasses.push_back(std::make_unique<LightPosPass>());
    m_renderPasses.push_back(std::make_unique<BackgroundPass>());

    for (const auto& pass : m_renderPasses) {
        if (pass) pass->init(m_renderContext.funcs);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glCullFace(GL_BACK);

    emit glInitialized();
}

void BlocksRenderer::resizeGL(int w, int h) {
    m_renderContext.screenWidth = w;
    m_renderContext.screenHeight = h;

    glViewport(0, 0, w, h);
    m_renderContext.camera->setAspectRatio((float)w / (float)h);
}

void BlocksRenderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& pass : m_renderPasses) {
        if (pass) pass->render(m_renderContext);
    }

    // updateLight();
}

void BlocksRenderer::setBuffer(const QVector<Color> &blocks) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderContext.ssbo.bufferId());
    glBufferData(GL_SHADER_STORAGE_BUFFER, blocks.size() * sizeof(Color), blocks.constData(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void BlocksRenderer::initBuffers() {
    if (!m_renderContext.vao.create()) qWarning("Failed to create vao");
    m_renderContext.vao.bind();

    if (!m_renderContext.vbo.create()) qWarning("Failed to create vbo");
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

    if (!m_renderContext.ssbo.create()) {
        qWarning("Failed to create ssbo");
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_renderContext.ssbo.bufferId());
}

void BlocksRenderer::initTexture() {
    m_renderContext.texture = std::make_unique<QOpenGLTexture>(QImage(":/assets/texture.png"));
    m_renderContext.texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_renderContext.texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_renderContext.texture->generateMipMaps();

    if (!m_renderContext.texture->isCreated()) qWarning() << "Texture not created!";
}

void BlocksRenderer::initNormalMap() {
    m_renderContext.normalMap = std::make_unique<QOpenGLTexture>(QImage(":/assets/normal.png"));
    m_renderContext.normalMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_renderContext.normalMap->setMagnificationFilter(QOpenGLTexture::Linear);
    m_renderContext.normalMap->generateMipMaps();

    if (!m_renderContext.normalMap->isCreated()) qWarning() << "Normal map not created!";
}

void BlocksRenderer::initShadowMap() {
    m_renderContext.depthMap = std::make_unique<QOpenGLTexture>(QOpenGLTexture::Target2D);
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
    m_renderContext.fbo = std::make_unique<QOpenGLFramebufferObject>(QSize(SHADOW_WIDTH, SHADOW_HEIGHT), format);

    GLuint fboId = m_renderContext.fbo->handle();
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_renderContext.depthMap->textureId(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) qWarning("Framebuffer not complete!");
}

void BlocksRenderer::initCubeMap() {
    m_renderContext.cubeMap = std::make_unique<QOpenGLTexture>(QOpenGLTexture::TargetCubeMap);
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

    m_renderContext.cubeMap->generateMipMaps();
    m_renderContext.cubeMap->release();

    if (!m_renderContext.cubeMap->isCreated()) qWarning() << "Cubemap not created!";
}

void BlocksRenderer::initLight() {
    m_renderContext.light = std::make_unique<Camera>(Camera::ProjectionMode::Orthogonal);
    m_renderContext.light->setNearPlane(10.0f);
    m_renderContext.light->setFarPlane(40.0f);
    m_renderContext.light->setSize(30.0f);
    m_renderContext.light->setPosition({5.0f, 30.0f, 5.0f});
    m_renderContext.light->lookAt({5.0f, 1.0f, 5.0f});
}

void BlocksRenderer::updateLight() {
    static float time = 0.0f;
    time = time + 0.016f;
    float z = sin(time * 0.1) * 10.0f;
    float x = cos(time * 0.1) * 10.0f;
    m_renderContext.light->setPosition({5.0f + x, 30.0f, 5.0f + z});
    m_renderContext.light->lookAt({5.0f, 1.0f, 5.0f});
}
