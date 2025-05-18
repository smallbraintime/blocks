#include "assetmanager.h"

#include <QOpenGLFunctions>

Mesh::Mesh(const QVector<float> &verticies, const QVector<unsigned int>& indices) {
    m_vbo.create();
    if (!m_vbo.isCreated()) {
        qWarning("Failed to create Vbo.");
        return;
    }
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(verticies.data(), verticies.size() * sizeof(float));


    m_ebo.create();
    if (!m_ebo.isCreated()) {
        qWarning("Failed to create Ebo.");
        return;
    }
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ebo.bind();
    m_ebo.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if (f) {
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    }

    m_vbo.release();
    m_ebo.release();
}

Mesh::~Mesh() {
    if (m_vbo.isCreated()) m_vbo.destroy();
    if (m_ebo.isCreated()) m_ebo.destroy();
}

void Mesh::beginFrame(QOpenGLShaderProgram &program) {
    m_vbo.bind();
    m_ebo.bind();

    program.enableAttributeArray(0);
    program.setAttributeBuffer(0, GL_FLOAT, 0, 3);
}

void Mesh::endFrame(QOpenGLShaderProgram &program) {
    m_vbo.release();
    m_ebo.release();
}


void Material::beginFrame(QOpenGLShaderProgram &program) {
    program.setUniformValue("uColor", m_color);

    if (m_isTransparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    }
}

void Material::endFrame(QOpenGLShaderProgram &program) {
    if (m_isTransparent) {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
}
