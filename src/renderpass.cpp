#include "renderpass.h"

#include "renderer.h"
#include "data.h"

void BackgroundPass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/skybox.vert"))
        qWarning("Failed to add vertex shader.");

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/skybox.frag"))
        qWarning("Failed to add fragment shader.");

    if (!m_shaderProgram.link()) qWarning("Failed to link shaders.");

    m_viewProjUniform = m_shaderProgram.uniformLocation("uViewProj");
}

void BackgroundPass::render(RenderContext &renderContext) {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    m_shaderProgram.bind();
    renderContext.vao.bind();
    renderContext.cubeMap->bind(0);

    QMatrix4x4 view = renderContext.camera->view();
    view.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    QMatrix4x4 viewProj = renderContext.camera->projection() * view;

    m_shaderProgram.setUniformValue(m_viewProjUniform, viewProj);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_shaderProgram.release();
    renderContext.vao.release();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void BasePass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_baseShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/base.vert"))
        qWarning("Failed to add vertex shader.");

    if (!m_baseShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/base.frag"))
        qWarning("Failed to add fragment shader.");

    if (!m_baseShaderProgram.link()) qWarning("Failed to link shaders.");

    if (!m_wireframeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/base.vert"))
        qWarning("Failed to add vertex shader.");

    if (!m_wireframeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.frag"))
        qWarning("Failed to add fragment shader.");

    if (!m_wireframeShaderProgram.link()) qWarning("Failed to link shaders.");

    m_uniforms.viewProj = m_baseShaderProgram.uniformLocation("uViewProj");
    m_uniforms.cameraPos = m_baseShaderProgram.uniformLocation("uCameraPos");
    m_uniforms.lightViewProj = m_baseShaderProgram.uniformLocation("uLightViewProj");
    m_uniforms.lightPos = m_baseShaderProgram.uniformLocation("uLightPos");
    m_uniforms.color = m_wireframeShaderProgram.uniformLocation("uColor");

    m_wireframeShaderProgram.bind();
    m_wireframeShaderProgram.setUniformValue(m_uniforms.color, QVector3D{0.0f, 0.0f, 0.0f});
    m_wireframeShaderProgram.release();
}


void BasePass::render(RenderContext &renderContext) {
    renderBase(renderContext);
    renderWireframe(renderContext);
}

void BasePass::renderBase(RenderContext &renderContext) {
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    m_baseShaderProgram.bind();
    renderContext.vao.bind();
    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId());
    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, renderContext.ssbo.bufferId());
    renderContext.normalMap->bind(0);
    renderContext.depthMap->bind(1);
    renderContext.texture->bind(2);
    renderContext.cubeMap->bind(3);

    QMatrix4x4 viewProj = renderContext.camera->projection() * renderContext.camera->view();
    QMatrix4x4 lightViewProj = renderContext.light->projection() * renderContext.light->view();

    m_baseShaderProgram.setUniformValue(m_uniforms.viewProj, viewProj);
    m_baseShaderProgram.setUniformValue(m_uniforms.cameraPos, renderContext.camera->position());
    m_baseShaderProgram.setUniformValue(m_uniforms.lightViewProj, lightViewProj);
    m_baseShaderProgram.setUniformValue(m_uniforms.lightPos, renderContext.light->position());

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
    m_baseShaderProgram.release();

    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);
}

void BasePass::renderWireframe(RenderContext &renderContext) {
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.0f, -1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_wireframeShaderProgram.bind();
    renderContext.vao.bind();
    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId());
    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, renderContext.ssbo.bufferId());

    QMatrix4x4 viewProj = renderContext.camera->projection() * renderContext.camera->view();

    m_wireframeShaderProgram.setUniformValue(m_uniforms.viewProj, viewProj);

    renderContext.funcs->glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1000);

    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    renderContext.vao.release();
    m_wireframeShaderProgram.release();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
}

void ShadowMapPass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shadowmap.vert"))
        qWarning("Failed to add vertex shader.");

    if (!m_shaderProgram.link()) qWarning("Failed to link shaders.");

    m_viewProjUniform = m_shaderProgram.uniformLocation("uViewProj");
}

void ShadowMapPass::render(RenderContext& renderContext) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    renderContext.funcs->glBindFramebuffer(GL_FRAMEBUFFER, renderContext.fbo->handle());
    glClear(GL_DEPTH_BUFFER_BIT);
    m_shaderProgram.bind();
    renderContext.vao.bind();
    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderContext.ssbo.bufferId());
    renderContext.funcs->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, renderContext.ssbo.bufferId());

    m_shaderProgram.setUniformValue(m_viewProjUniform, renderContext.light->projection() * renderContext.light->view());

    renderContext.funcs->glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 1000);

    renderContext.funcs->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    renderContext.fbo->release();
    m_shaderProgram.release();
    renderContext.vao.release();

    glViewport(0, 0, renderContext.m_screenWidth, renderContext.m_screenHeight);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
}

void LightDepthPass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lightdepth.vert"))
        qWarning("Failed to add vertex shader.");

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lightdepth.frag"))
        qWarning("Failed to add fragment shader.");

    if (!m_shaderProgram.link()) qWarning("Failed to link shaders.");

    if (!m_vao.create()) qWarning("Failed to create vao");
}

void LightDepthPass::render(RenderContext &renderContext) {
    glDepthMask(GL_FALSE);

    m_shaderProgram.bind();
    m_vao.bind();

    renderContext.depthMap->bind(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_vao.release();
    renderContext.depthMap->release();
    m_shaderProgram.release();

    glDepthMask(GL_TRUE);
}


void LightPosPass::init(QOpenGLFunctions_4_3_Core* funcs) {
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lightpos.vert"))
        qWarning("Failed to add vertex shader.");

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.frag"))
        qWarning("Failed to add fragment shader.");

    if (!m_shaderProgram.link()) qWarning("Failed to link shaders.");

    m_uniforms.viewProj = m_shaderProgram.uniformLocation("uViewProj");
    m_uniforms.lightPos = m_shaderProgram.uniformLocation("uLightPos");
    m_uniforms.color = m_shaderProgram.uniformLocation("uColor");

    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue(m_uniforms.color, QVector3D{1.0f, 1.0f, 1.0f});
    m_shaderProgram.release();
}

void LightPosPass::render(RenderContext& renderContext) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    m_shaderProgram.bind();
    renderContext.vao.bind();

    m_shaderProgram.setUniformValue(m_uniforms.viewProj, renderContext.camera->projection() * renderContext.camera->view());
    m_shaderProgram.setUniformValue(m_uniforms.lightPos, renderContext.light->position());

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_shaderProgram.release();
    renderContext.vao.release();

    glDisable(GL_MULTISAMPLE);
    glDisable(GL_CULL_FACE);
}
