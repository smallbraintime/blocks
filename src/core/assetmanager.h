#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QColor>
#include <QStringView>
#include <QHash>
#include <QWeakPointer>
#include <QOpenGLFunctions>

class Mesh : QOpenGLFunctions {
public:
    enum class DrawType {
        Static,
        Dynamic
    };

    explicit Mesh(const QVector<float>& verticies, const QVector<unsigned int>& indices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
};

class AssetManager {
public:
    AssetManager() = default;

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    template<typename... Args>
    const Mesh* createAsset(QStringView name, Args&&... args) {
        return m_meshes.insert(name, std::forward<Args>(args)...);
    }

    const Mesh* getAsset(QStringView name) const {
        auto it = m_meshes.constFind(name);
        return (it != m_meshes.cend()) ? &it.value() : nullptr;
    }

    template<typename T>
    bool hasAsset(QStringView name) const {
        return m_meshes.contains(name);
    }

    template<typename T>
    void removeAsset(QStringView name) {
        m_meshes.remove(name);
    }

private:
    QHash<QStringView, Mesh> m_meshes;
};
