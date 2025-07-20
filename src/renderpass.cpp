#include "renderpass.h"

#include <QImage>
#include <QDir>

#include "renderer.h"
#include "data.h"

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

    m_uniformLocations.view = m_shaderProgram.uniformLocation("uView");
    m_uniformLocations.projection = m_shaderProgram.uniformLocation("uProjection");
    m_uniformLocations.cubeMap = m_shaderProgram.uniformLocation("uCubeMap");
}

void BackgroundPass::render(RenderContext &renderContext) {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    m_shaderProgram.bind();
    renderContext.vao.bind();

    renderContext.cubeMap->bind(0);
    m_shaderProgram.setUniformValue(m_uniformLocations.cubeMap, 0);

    QMatrix4x4 view = renderContext.camera->view();
    view.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    m_shaderProgram.setUniformValue(m_uniformLocations.view, view);
    m_shaderProgram.setUniformValue(m_uniformLocations.projection, renderContext.camera->projection());

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_shaderProgram.release();
    renderContext.vao.release();
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
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
    m_uniformLocations.normalMap = m_shaderProgram.uniformLocation("uNormalMap");
    m_uniformLocations.texture = m_shaderProgram.uniformLocation("uTexture");
    m_uniformLocations.depthMap = m_shaderProgram.uniformLocation("uDepthMap");
    m_uniformLocations.lightViewProj = m_shaderProgram.uniformLocation("uLightViewProj");
    m_uniformLocations.lightPos = m_shaderProgram.uniformLocation("uLightPos");
    m_uniformLocations.cubeMap = m_shaderProgram.uniformLocation("uCubeMap");
}


void BasePass::render(RenderContext &renderContext) {
    glEnable(GL_CULL_FACE);
    m_shaderProgram.bind();
    renderContext.vao.bind();
    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId());
    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, renderContext.ssbo.bufferId());

    renderContext.normalMap->bind(0);
    m_shaderProgram.setUniformValue(m_uniformLocations.normalMap, 0);
    renderContext.depthMap->bind(1);
    m_shaderProgram.setUniformValue(m_uniformLocations.depthMap, 1);
    renderContext.texture->bind(2);
    m_shaderProgram.setUniformValue(m_uniformLocations.texture, 2);
    renderContext.cubeMap->bind(3);
    m_shaderProgram.setUniformValue(m_uniformLocations.cubeMap, 3);

    m_shaderProgram.setUniformValue(m_uniformLocations.view, renderContext.camera->view());
    m_shaderProgram.setUniformValue(m_uniformLocations.projection, renderContext.camera->projection());
    m_shaderProgram.setUniformValue(m_uniformLocations.viewPos, renderContext.camera->position());
    m_shaderProgram.setUniformValue(m_uniformLocations.lightViewProj, renderContext.light->projection() * renderContext.light->view());
    m_shaderProgram.setUniformValue(m_uniformLocations.lightPos, renderContext.light->position());

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

void ShadowMapPass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shadowmap.vert")) {
        qWarning("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.link()) {
        qWarning("Failed to link shaders.");
    }

    m_uniformLocations.view = m_shaderProgram.uniformLocation("uView");
    m_uniformLocations.projection = m_shaderProgram.uniformLocation("uProjection");
}

void ShadowMapPass::render(RenderContext& renderContext) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glCullFace(GL_FRONT);
    renderContext.funcs->glBindFramebuffer(GL_FRAMEBUFFER, renderContext.fbo->handle());
    glClear(GL_DEPTH_BUFFER_BIT);

    m_shaderProgram.bind();
    renderContext.vao.bind();
    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId());
    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, renderContext.ssbo.bufferId());

    m_shaderProgram.setUniformValue(m_uniformLocations.view, renderContext.light->view());
    m_shaderProgram.setUniformValue(m_uniformLocations.projection, renderContext.light->projection());

    renderContext.funcs->glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1000);

    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    renderContext.fbo->release();
    m_shaderProgram.release();
    renderContext.vao.release();
    glCullFace(GL_BACK);
    glViewport(0, 0, renderContext.m_screenWidth, renderContext.m_screenHeight);
}

void LightDepthPass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lightdepth.vert")) {
        qWarning("Failed to add vertex shader.");
    }
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lightdepth.frag")) {
        qWarning("Failed to add fragment shader.");
    }
    if (!m_shaderProgram.link()) {
        qWarning("Failed to link shaders.");
    }

    if (!m_vao.create()) {
        qWarning("Failed to create vao");
    }
}

void LightDepthPass::render(RenderContext &renderContext) {
    qDebug() << renderContext.light->position() << renderContext.light->orientation().toEulerAngles();
    glDepthMask(GL_FALSE);
    m_shaderProgram.bind();
    m_vao.bind();

    renderContext.depthMap->bind(0);
    int depthLoc = m_shaderProgram.uniformLocation("depthTexture");
    m_shaderProgram.setUniformValue(depthLoc, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_vao.release();
    renderContext.depthMap->release();
    m_shaderProgram.release();

    glDepthMask(GL_TRUE);
}

