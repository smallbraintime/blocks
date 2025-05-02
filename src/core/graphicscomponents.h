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
    explicit StaticMesh(const QMatrix4x4& position, QSharedPointer<Mesh> mesh, QSharedPointer<Material> material)
        : m_positions({position}), m_mesh(mesh), m_material(material) {}
    explicit StaticMesh(QSharedPointer<Mesh> mesh, QSharedPointer<Material> material)
        : m_mesh(mesh), m_material(material) {}

    void setMesh(QSharedPointer<Mesh> mesh) { m_mesh = std::move(mesh); }
    void setMaterial(QSharedPointer<Material> material) { m_material = std::move(material); }
    void addPosition(const QMatrix4x4& position) { m_positions.append(position); }
    void removePosition(size_t index) { m_positions.removeAt(index); }
    QMatrix4x4& getPosition(size_t index) { return m_positions[index]; }

    QSharedPointer<Mesh> mesh() { return m_mesh; }
    QSharedPointer<Material> material() { return m_material; }

    void beginFrame(QOpenGLShaderProgram &program) override;
    void endFrame(QOpenGLShaderProgram &program) override;

private:
    QVector<QMatrix4x4> m_positions;
    QSharedPointer<Mesh> m_mesh;
    QSharedPointer<Material> m_material;
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

    void beginFrame(QOpenGLShaderProgram &program) override;
    void endFrame(QOpenGLShaderProgram &program) override;

private:
    LightType m_type;
    QVector3D m_position;
    QVector3D m_direction;
    QColor m_color;
    float m_intensity;
};
