#include "assetmanager.h"

#include <QOpenGLFunctions>

Mesh::Mesh(const QVector<float> &verticies, const QVector<unsigned int>& indices) {
    if (!m_vao.create()) {
        qWarning("Failed to create Vbo.");
        return;
    }
    m_vao.bind();

    if (!m_vbo.create()) {
        qWarning("Failed to create Vbo.");
        return;
    }
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(verticies.data(), verticies.size() * sizeof(float));


    if (!m_ebo.create()) {
        qWarning("Failed to create Ebo.");
        return;
    }
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ebo.bind();
    m_ebo.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    m_vao.release();
}

Mesh::~Mesh() {
    if (m_vbo.isCreated()) m_vbo.destroy();
    if (m_ebo.isCreated()) m_ebo.destroy();
    if (m_vao.isCreated()) m_vao.destroy();
}
