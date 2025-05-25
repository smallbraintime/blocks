#include "renderer.h"

#include "camera.h"

void Renderer::initializeGL() {
    initializeOpenGLFunctions();


    glEnable(GL_DEPTH_TEST);
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_sceneData->activeCamera->setAspectRatio(w / h);
}

void Renderer::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if (m_isTransparent) {
    //     glEnable(GL_BLEND);
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //     glDepthMask(GL_FALSE);
    // } else {
    //     glDisable(GL_BLEND);
    //     glDepthMask(GL_TRUE);
    // }

    update();
}

ZPass::ZPass() {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/zpass.vert")) {
        qFatal("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/zpass.frag")) {
        qFatal("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qFatal("Failed to link shaders.");
    }
}

void ZPass::begin() {

}

void ZPass::render(const Scene::SceneData &sceneData) {

}

void ZPass::end() {

}

BasePass::BasePass() {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/base.vert")) {
        qFatal("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/base.frag")) {
        qFatal("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qFatal("Failed to link shaders.");
    }
}

void BasePass::begin() {

}

void BasePass::render(const Scene::SceneData &sceneData) {

}

void BasePass::end() {

}

LightPass::LightPass() {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/postprocess.vert")) {
        qFatal("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/postprocess.frag")) {
        qFatal("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qFatal("Failed to link shaders.");
    }
}

void LightPass::begin() {

}

void LightPass::render(const Scene::SceneData &sceneData) {

}

void LightPass::end() {

}
