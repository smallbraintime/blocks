#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>


class Camera {
public:
    Camera() {};
    Camera(const QVector3D &position, const QQuaternion &orientation)
        : m_position(position), m_orientation(orientation) {}

    void set_position(const QVector3D& position);
    void set_orientation(const QVector3D& euler_angles);
    void translate(const QVector3D& offset);
    void rotate(const QVector3D& axis, float angle);

    QMatrix4x4 view() const;
    const QVector3D &position() const { return m_position; }
    const QQuaternion &orientation() const { return m_orientation; }

private:
    QVector3D m_position;
    QQuaternion m_orientation;
};
