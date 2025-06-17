#include "renderpass.h"

#include <QOpenGLFunctions_4_3_Core>

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

    m_uniformLocations.view = m_shaderProgram.uniformLocation("uView");
    m_uniformLocations.projection = m_shaderProgram.uniformLocation("uProjection");
    if (m_uniformLocations.view == -1) {
        qWarning("Could not find uniform 'uView'");
    }
    if (m_uniformLocations.projection == -1) {
        qWarning("Could not find uniform 'uProjection'");
    }
}

void BasePass::render(RenderContext &renderContext) {
    m_shaderProgram.bind();
    renderContext.vao.bind();
    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId());
    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, renderContext.ssbo.bufferId());

    m_shaderProgram.setUniformValue(m_uniformLocations.view, renderContext.camera->view());
    m_shaderProgram.setUniformValue(m_uniformLocations.projection, renderContext.camera->projection());

    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, renderContext.ssbo.bufferId());
    renderContext.funcs->glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr , 10000);
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

    // TODO: draw a dummy triangle to trigger fragment shader

    m_shaderProgram.release();
}
