#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QColor>
#include <QStringView>
#include <QMap>

class Mesh {
public:
    explicit Mesh(const QVector<float>& verticies, const std::optional<QVector<unsigned int>>& indices);
    ~Mesh();

    void bind() { m_vao.bind(); }
    void unbind() { m_vao.release(); }

private:
    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
    QOpenGLVertexArrayObject m_vao;
    bool m_valid;
};

class Material {
public:
    Material() = default;

    void setColor(const QColor& color) { m_color = color; }

    const QColor& color() const { return m_color; }

private:
    QColor m_color;
};

class AssetManager {
public:
    AssetManager() = default;

    template<typename T>
    void addAsset(QStringView id, QSharedPointer<T> asset) {
    }

    template<typename T>
    QSharedPointer<T> getAsset(QStringView id) const {
    }

    template<typename T>
    void removeAsset(QStringView id) {
    }

private:
    QMap<QStringView, QSharedPointer<Mesh>> m_meshes;
    QMap<QStringView, QSharedPointer<Material>> m_materials;
};

// class Texture {
// public:
//     explicit Texture(const std::vector<char>& pixels, size_t width, size_t height);
//     ~Texture() { if (m_texture.isCreated()) m_texture.destroy(); };

// private:
//     QOpenGLTexture m_texture;
//     bool m_valid;
// };
