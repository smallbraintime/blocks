#include "camera.h"

void Camera::set_position(const QVector3D& position) {
    m_position = position;
}

void Camera::set_orientation(const QVector3D& euler_angles) {
    m_orientation = QQuaternion::fromEulerAngles(euler_angles);
}

void Camera::translate(const QVector3D &offset) {
    QVector3D right = m_orientation.rotatedVector(QVector3D(1.0f, 0.0f, 0.0f));
    QVector3D up = m_orientation.rotatedVector(QVector3D(0.0f, 1.0f, 0.0f));
    QVector3D front = m_orientation.rotatedVector(QVector3D(0.0f, 0.0f, -1.0f));

    m_position += right * offset.x();
    m_position += up * offset.y();
    m_position += front * offset.z();
}

void Camera::rotate(const QVector3D& axis, float angle) {
    QQuaternion localRotation = QQuaternion::fromAxisAndAngle(axis, angle);
    m_orientation = m_orientation * localRotation;
    m_orientation.normalize();
}

QMatrix4x4 Camera::view() const {
    QMatrix4x4 view_matrix;
    view_matrix.rotate(m_orientation.conjugated());
    view_matrix.translate(-m_position);
    return view_matrix;
}

QMatrix4x4 Camera::projection() const {
    QMatrix4x4 projection;
    projection.setToIdentity();

    switch (m_mode) {
    case ProjectionMode::Perspective:
        projection.perspective(m_viewFactor, m_aspectRatio, m_nearPlane, m_farPlane);
        break;
    case ProjectionMode::Orthogonal:
        float half_width = m_viewFactor * m_aspectRatio * 0.5f;
        float half_height = m_viewFactor * 0.5f;
        projection.ortho(-half_width, half_width, -half_height, half_height, m_nearPlane, m_farPlane);
        break;
    }

    return projection;
}

void Camera::beginFrame(QOpenGLShaderProgram &program) {
    program.setUniformValue("uView", view());
    program.setUniformValue("uProjection", projection());
}

void Camera::endFrame(QOpenGLShaderProgram &program) {}
