#include "renderpass.h"

#include "renderer.h"

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

void ZPass::render(const RenderContext &renderContext) {

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

void BasePass::render(const RenderContext &renderContext) {

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

void LightPass::render(const RenderContext &renderContext) {

}

void LightPass::end() {

}
