#include "renderpass.h"

#include "renderer.h"

void BackgroundPass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/background.vert")) {
        qWarning("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/background.frag")) {
        qWarning("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qWarning("Failed to link shaders.");
    }
}

void BackgroundPass::render(RenderContext &renderContext) {
    glDisable(GL_DEPTH_TEST);
    m_shaderProgram.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_shaderProgram.release();
    glEnable(GL_DEPTH_TEST);
}

void BasePass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/base.vert")) {
        qWarning("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/base.frag")) {
        qWarning("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qWarning("Failed to link shaders.");
    }

    m_uniformLocations.view = m_shaderProgram.uniformLocation("uView");
    m_uniformLocations.projection = m_shaderProgram.uniformLocation("uProjection");
    m_uniformLocations.viewPos = m_shaderProgram.uniformLocation("uViewPos");
    if (m_uniformLocations.view == -1) {
        qWarning("Could not find uniform 'uView'");
    }
    if (m_uniformLocations.projection == -1) {
        qWarning("Could not find uniform 'uProjection'");
    }
    if (m_uniformLocations.viewPos == -1) {
        qWarning("Could not find uniform 'uViewPos'");
    }
}

void BasePass::render(RenderContext &renderContext) {
    glEnable(GL_CULL_FACE);
    m_shaderProgram.bind();
    renderContext.vao.bind();
    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId());
    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, renderContext.ssbo.bufferId());
    renderContext.normalMap->bind();

    m_shaderProgram.setUniformValue(m_uniformLocations.view, renderContext.camera->view());
    m_shaderProgram.setUniformValue(m_uniformLocations.projection, renderContext.camera->projection());
    m_shaderProgram.setUniformValue(m_uniformLocations.viewPos, renderContext.camera->position());

    renderContext.funcs->glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1000);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    // TODO: fix this shit
    // renderContext.pointedBlock
    // glBindBufferRange(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId(), 0, 0, sizeof(Color));
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glBindBufferRange(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId(), 0, 0, sizeof(Color));
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glBindBufferRange(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId(), 0, 0, sizeof(Color));
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    renderContext.vao.release();
    m_shaderProgram.release();
    glDisable(GL_CULL_FACE);
}
