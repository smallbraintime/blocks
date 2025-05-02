#include "graphicscomponents.h"

#include <QOpenGLFunctions>

Mesh::Mesh(const QVector<float> &verticies, const std::optional<QVector<unsigned int>>& indices) {
    m_vao.create();
    if (!m_vao.isCreated()) {
        qWarning("Failed to create Vao.");
        return;
    }
    m_vao.bind();

    m_vbo.create();
    if (!m_vbo.isCreated()) {
        qWarning("Failed to create Vbo.");
        return;
    }
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(verticies.data(), verticies.size() * sizeof(float));

    if (indices) {
        m_ebo.create();
        if (!m_ebo.isCreated()) {
            qWarning("Failed to create Ebo.");
            return;
        }
        m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_ebo.bind();
        m_ebo.allocate(indices->data(), indices->size() * sizeof(unsigned int));
    }

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if (f) {
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    }

    m_vbo.release();
    m_ebo.release();
    m_vao.release();

    m_valid = true;
}

Mesh::~Mesh() {
    if (m_vao.isCreated()) m_vao.destroy();
    if (m_vbo.isCreated()) m_vbo.destroy();
    if (m_ebo.isCreated()) m_ebo.destroy();
}

Texture::Texture(const std::vector<char>& pixels, size_t width, size_t height) : m_texture(QOpenGLTexture::Target::Target2D) {
    m_texture.create();
    m_texture.setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    m_texture.setSize(width, height, 1);
    m_texture.setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_texture.allocateStorage();
    m_texture.setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, pixels.data());
    m_texture.generateMipMaps(GL_TEXTURE_2D);

    if (!m_texture.isCreated()) {
        qWarning("Failed to create a texture.");
        return;
    }

    m_valid = true;
}
