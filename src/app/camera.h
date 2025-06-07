#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

class Camera {
public:
    enum class ProjectionMode : quint8 {
        Perspective,
        Orthogonal
    };

    Camera() = default;
    explicit Camera(float aspectRatio, const QVector3D &position, const QQuaternion &orientation)
        : m_aspectRatio(aspectRatio), m_position(position), m_orientation(orientation) {}

    void set_position(const QVector3D& position);
    void set_orientation(const QVector3D& euler_angles);
    void translate(const QVector3D& offset);
    void rotate(const QVector3D& axis, float angle);
    void setFov(float fov) { m_viewFactor = fov; }
    void setSize(float size) { m_viewFactor = size; }
    void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
    void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
    void setfarPlane(float farPlane) { m_farPlane = farPlane; }
    void setProjectionMode(ProjectionMode mode) { m_mode = mode; }

    QMatrix4x4 view() const;
    const QVector3D& position() const { return m_position; }
    const QQuaternion& orientation() const { return m_orientation; }
    QMatrix4x4 projection() const;
    float fov() const { return m_viewFactor; }
    float size() const { return m_viewFactor; }
    float aspectRatio() const { return m_aspectRatio; }
    float nearPlane() const { return m_nearPlane; }
    float farPlane() const { return m_farPlane; }

private:
    QVector3D m_position;
    QQuaternion m_orientation;
    float m_viewFactor = 45.0f; // can be treated as a fov or size depends on camera mode
    float m_aspectRatio;
    float m_nearPlane = 1.0f;
    float m_farPlane = 1000.0f;
    ProjectionMode m_mode{ProjectionMode::Perspective};
};
