#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QRandomGenerator64>
#include <QMatrix4x4>
#include <QSharedPointer>
#include <QVector3D>
#include <QOpenGLShaderProgram>

#include "assetmanager.h"

class StaticMesh{
    explicit StaticMesh(QSharedPointer<Mesh> mesh, const QColor& color)
        : m_mesh(mesh), m_color(color) {}

    void setMesh(QSharedPointer<Mesh> mesh) { m_mesh = std::move(mesh); }
    void setColor(const QColor& color) { m_color = color; }
    void setIsTransparent(bool isTransparent) { m_isTransparent = isTransparent; }
    const QMatrix4x4& getPosition() { return m_transform; }

    QSharedPointer<Mesh> mesh() { return m_mesh; }
    const QColor& color() { return m_color; }
    bool isTransparent() { return m_isTransparent; }

    QMatrix4x4 m_transform;

private:
    QSharedPointer<Mesh> m_mesh;
    QColor m_color;
    bool m_isTransparent;

    friend class Renderer;
};

class InstancedStaticMesh {
    struct StaticMeshInstance {
        QMatrix4x4 m_transform;
        QColor m_color;
    };

public:
    explicit InstancedStaticMesh(QSharedPointer<Mesh> mesh) : m_mesh(mesh) {}
    ~InstancedStaticMesh();

    void setMesh(QSharedPointer<Mesh> mesh) { m_mesh = std::move(mesh); }
    void addInstance(const QMatrix4x4& position, const QColor& color);
    void removeInstanceAt(size_t index);
    void setTransformAt(size_t index, const QMatrix4x4& transform);
    void setColorAt(size_t index, const QColor& color);
    void setIsTransparent(bool isTransparent) { m_isTransparent  = isTransparent; }

    const StaticMeshInstance& getInstanceAt(size_t index) const { return m_instances[index]; }
    const QSharedPointer<Mesh> mesh() const { return m_mesh; }
    bool isTransparent() const { return m_isTransparent; }

    QMatrix4x4 m_transform;

private:
    QVector<StaticMeshInstance> m_instances;
    QSharedPointer<Mesh> m_mesh;
    bool m_isTransparent;
    QOpenGLBuffer m_ssbo;
    bool m_isModified;

    friend class Renderer;
};

class Light {
public:
    enum class LightType {
        Directional,
        Point,
        Spot
    };

    explicit Light(LightType type, const QColor& color, float intensity, const QVector3D& direction)
        : m_type(type), m_direction(direction), m_color(color), m_intensity(intensity) {}

    void setType(LightType type) { m_type = type; };
    void setDirection(const QVector3D &direction) { m_direction = direction; };
    void setColor(const QColor &color) { m_color = color; };
    void setIntensity(float intensity) { m_intensity = intensity; };

    LightType type() const {  return m_type; };
    QVector3D direction() const { return m_direction; };
    QColor color() const { return m_color; };
    float intensity() const { return m_intensity; };

    QMatrix4x4 m_transform;

private:
    LightType m_type;
    QVector3D m_direction;
    QColor m_color;
    float m_intensity;

    friend class Renderer;
};
