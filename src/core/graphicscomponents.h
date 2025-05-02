#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QRandomGenerator64>

#include "graphicscomponent.h"

class Mesh : GraphicsComponent {
public:
    explicit Mesh(const QVector<float>& verticies, const std::optional<QVector<unsigned int>>& indices);
    ~Mesh();

private:
    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
    QOpenGLVertexArrayObject m_vao;
    bool m_valid;

    void bind() { m_vao.bind(); }
    void unbind() { m_vao.release(); }
};

class Texture {
public:
    explicit Texture(const std::vector<char>& pixels, size_t width, size_t height);
    ~Texture() { if (m_texture.isCreated()) m_texture.destroy(); };

private:
    QOpenGLTexture m_texture;
    bool m_valid;
};

class Material {
public:
    Material() {};

    void setColor(const QColor& color) { m_color = color; }
    void setTexture(QSharedPointer<Texture> texture) { m_texture = texture; }

    QColor color() { return m_color; }

private:
    QColor m_color;
    QSharedPointer<Texture> m_texture = nullptr;
};

class RenderObject {
public:
    RenderObject() {}

    RenderObject(const RenderObject& renderObject);
    RenderObject& operator=(const RenderObject& renderObject);

    void setTransform(const QMatrix4x4& transform) { m_transform = transform; }
    void setMesh(QSharedPointer<Mesh> mesh) { m_mesh = mesh; }
    void setMaterial(QSharedPointer<Material> material) { m_material = material; }

    const QMatrix4x4& transform() { return m_transform; }
    QSharedPointer<Mesh> mesh() const { return m_mesh; }
    QSharedPointer<Material> material() const { return m_material; }

private:
    QMatrix4x4 m_transform;
    QSharedPointer<Mesh> m_mesh;
    QSharedPointer<Material> m_material;
};

class Light {
public:
    enum class LightType {
        Directional,
        Point,
        Spot
    };

    Light(LightType type, const QVector3D& position, const QColor& color, float intensity, const QVector3D& direction = QVector3D(0,-1,0))
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

private:
    LightType m_type;
    QVector3D m_position;
    QVector3D m_direction;
    QColor m_color;
    float m_intensity;
};
