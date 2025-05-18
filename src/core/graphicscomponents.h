#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QRandomGenerator64>
#include <QMatrix4x4>
#include <QSharedPointer>
#include <QVector3D>
#include <QOpenGLShaderProgram>

#include "graphicscomponent.h"
#include "assetmanager.h"

class StaticMesh : GraphicsComponent {
    explicit StaticMesh(const QMatrix4x4& transform, QSharedPointer<Mesh> mesh, QSharedPointer<Material> material)
        : m_transform(transform), m_mesh(mesh), m_material(material) {}

    void setMesh(QSharedPointer<Mesh> mesh) { m_mesh = std::move(mesh); }
    void setMaterial(QSharedPointer<Material> material) { m_material = std::move(material); }
    void setPosition(const QMatrix4x4& transform) { m_transform = transform; }
    const QMatrix4x4& getPosition() { return m_transform; }

    QSharedPointer<Mesh> mesh() { return m_mesh; }
    QSharedPointer<Material> material() { return m_material; }

    void beginFrame(QOpenGLShaderProgram& program) override;
    void endFrame(QOpenGLShaderProgram& program) override;

private:
    QMatrix4x4 m_transform;
    QSharedPointer<Mesh> m_mesh;
    QSharedPointer<Material> m_material;
};

class InstancedStaticMesh : GraphicsComponent {
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
    void setPositionAt(size_t index, const QMatrix4x4& transform);
    void setColorAt(size_t index, const QColor& color);
    void setIsTransparent(bool isTransparent) { m_isTransparent  = isTransparent; }

    const StaticMeshInstance& getInstanceAt(size_t index) const { return m_instances[index]; }
    const QSharedPointer<Mesh> mesh() const { return m_mesh; }
    bool isTransparent() const { return m_isTransparent; }

    void beginFrame(QOpenGLShaderProgram& program) override;
    void endFrame(QOpenGLShaderProgram& program) override;

private:
    QVector<StaticMeshInstance> m_instances;
    QSharedPointer<Mesh> m_mesh;
    bool m_isTransparent;
    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    bool m_isModified;
};

class Light : GraphicsComponent {
public:
    enum class LightType {
        Directional,
        Point,
        Spot
    };

    explicit Light(LightType type, const QVector3D& position, const QColor& color, float intensity, const QVector3D& direction = QVector3D(0,-1,0))
        : m_type(type), m_position(position), m_direction(direction), m_color(color), m_intensity(intensity) {}

    void setType(LightType type) { m_type = type; };
    void setPosition(const QVector3D &position) { m_position = position;};
    void setDirection(const QVector3D &direction) { m_direction = direction; };
    void setColor(const QColor &color) { m_color = color; };
    void setIntensity(float intensity) { m_intensity = intensity; };

    LightType type() const {  return m_type; };
    QVector3D position() const { return m_position; };
    QVector3D direction() const { return m_direction; };
    QColor color() const { return m_color; };
    float intensity() const { return m_intensity; };

    void beginFrame(QOpenGLShaderProgram& program) override;
    void endFrame(QOpenGLShaderProgram& program) override;

private:
    LightType m_type;
    QVector3D m_position;
    QVector3D m_direction;
    QColor m_color;
    float m_intensity;
};
