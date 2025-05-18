#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QColor>
#include <QStringView>
#include <QHash>
#include <QWeakPointer>

#include "graphicscomponent.h"

class Mesh : GraphicsComponent {
public:
    explicit Mesh(const QVector<float>& verticies, const QVector<unsigned int>& indices);
    ~Mesh();

    void init(QOpenGLVertexArrayObject& vao) override;
    void beginFrame(QOpenGLShaderProgram &program) override;
    void endFrame(QOpenGLShaderProgram &program) override;

private:
    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
};

class Material : GraphicsComponent {
public:
    Material() = default;

    void setColor(const QColor& color) { m_color = color; }
    void setIsTransparent(bool isTransparent) { m_isTransparent  = isTransparent; }

    const QColor& color() const { return m_color; }
    bool isTransparent() const { return m_isTransparent; }

    void beginFrame(QOpenGLShaderProgram &program) override;
    void endFrame(QOpenGLShaderProgram &program) override;z

private:
    QColor m_color;
    bool m_isTransparent;
    // QOpenGLTexture m_texture;
};

class AssetManager {
public:
    AssetManager() = default;

    template<typename T, typename... Args>
    T& createAsset(QStringView name, Args&&... args) {
        if constexpr (typeid(T) == typeid(Mesh)) {
            return m_meshes.insert(name, std::forward<Args>(args)...);
        } else if constexpr (typeid(T) == typeid(Material)) {
            return m_materials.insert(name, std::forward<Args>(args)...);
        } else {
            qWarning("Invalid type of the asset.");
        }
    }

    template<typename T>
    T& getAsset(QStringView name) const {
        if constexpr (typeid(T) == typeid(Mesh)) {
            return m_meshes.find(name);
        } else if constexpr (typeid(T) == typeid(Material)) {
            return m_materials.find(name);
        } else {
            qWarning("Invalid type of the asset.");
        }
    }

    template<typename T>
    bool hasAsset(QStringView name) const {
        if constexpr (typeid(T) == typeid(Mesh)) {
            return m_meshes.contains(name);
        } else if constexpr (typeid(T) == typeid(Material)) {
            return m_materials.contains(name);
        } else {
            qWarning("Invalid type of the asset.");
        }
    }

    template<typename T>
    void removeAsset(QStringView name) {
        if constexpr (typeid(T) == typeid(Mesh)) {
            m_meshes.remove(name);
        } else if constexpr (typeid(T) == typeid(Material)) {
            m_materials.find(name);
        } else {
            qWarning("Invalid type of the asset.");
        }
    }

private:
    QHash<QStringView, Mesh> m_meshes;
    QHash<QStringView, Material> m_materials;
};
