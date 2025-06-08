#include "renderpass.h"

#include "renderer.h"

BasePass::BasePass() {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/base.vert")) {
        qWarning("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/base.frag")) {
        qWarning("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qWarning("Failed to link shaders.");
    }

    m_uniformLocations.view = m_shaderProgram.attributeLocation("view");
    m_uniformLocations.projection = m_shaderProgram.attributeLocation("projection");
}

void BasePass::render(RenderContext &renderContext) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    m_shaderProgram.bind();
    renderContext.vao.bind();

    m_shaderProgram.setUniformValue(m_uniformLocations.view, renderContext.camera->view());
    m_shaderProgram.setUniformValue(m_uniformLocations.projection, renderContext.camera->projection());

    // TODO: fix this shit
    // renderContext.pointedBlock
    // glBindBufferRange(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId(), 0, 0, sizeof(Color));
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glBindBufferRange(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId(), 0, 0, sizeof(Color));
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glBindBufferRange(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId(), 0, 0, sizeof(Color));
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    renderContext.vao.release();
    m_shaderProgram.release();

    glDisable(GL_DEPTH_TEST);
}

FXAAPass::FXAAPass() {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/fxaa.vert")) {
        qWarning("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fxaa.frag")) {
        qWarning("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qWarning("Failed to link shaders.");
    }
}

void FXAAPass::render(RenderContext &renderContext) {
    m_shaderProgram.bind();

    // TODO: draw a dummy triangle

    m_shaderProgram.release();
}
