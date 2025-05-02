#include "renderer.h"

#include <QDateTime>

void Renderer::initializeGL() {
    initializeOpenGLFunctions();

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/basic.vert")) {
        throw std::runtime_error(m_shaderProgram.log().prepend("Failed to add vertex shader.").toStdString());
    }

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.frag")) {
        throw std::runtime_error(m_shaderProgram.log().prepend("Failed to add fragment shader.").toStdString());
    }

    if (!m_shaderProgram.link()) {
        throw std::runtime_error(m_shaderProgram.log().prepend("Failed to link shaders.").toStdString());
    }

    // do we have to create glcontext ?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_viewport->setAspectRatio(w / h);
}

void Renderer::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderProgram.bind();

    for (const auto renderObject : s_renderObjects) {

    }

    update();
}

QSharedPointer<RenderObject> Renderer::addRenderObject() {
    QSharedPointer<RenderObject> newObject(new RenderObject(), [](RenderObject *ptr) {
        s_freedIds.push_back(ptr->id());
        delete ptr;
    });

    if (s_freedIds.isEmpty()) {
        s_renderObjects.insert(newObject->id(), newObject);
    } else {
        s_renderObjects.insert(s_freedIds.takeLast(), newObject);
    }

    return newObject;
}

RenderObject::RenderObject(const RenderObject &renderObject) {
    this->m_transform = renderObject.m_transform;
    this->m_mesh = renderObject.m_mesh;
    this->m_material = renderObject.m_material;
    this->m_id = QRandomGenerator64::global()->generate();
}

RenderObject& RenderObject::operator=(const RenderObject &renderObject) {
    if (this != &renderObject) {
        this->m_transform = renderObject.m_transform;
        this->m_mesh = renderObject.m_mesh;
        this->m_material = renderObject.m_material;
        this->m_id = QRandomGenerator64::global()->generate();
    }
    return *this;
}
