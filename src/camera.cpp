#include "camera.h"

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

void Camera::lookAt(const QVector3D &target, const QVector3D& worldUp) {
    QVector3D z = (m_position - target).normalized();
    QVector3D x = QVector3D::crossProduct(worldUp, z).normalized();
    QVector3D y = QVector3D::crossProduct(z, x).normalized();

    QMatrix4x4 worldRotationMat;
    worldRotationMat.setColumn(0, QVector4D(x, 0.0f));
    worldRotationMat.setColumn(1, QVector4D(y, 0.0f));
    worldRotationMat.setColumn(2, QVector4D(z, 0.0f));
    worldRotationMat.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    m_orientation = QQuaternion::fromRotationMatrix(worldRotationMat.toGenericMatrix<3,3>());
}

QMatrix4x4 Camera::projection() const {
    QMatrix4x4 proj;
    proj.setToIdentity();
    switch (m_mode) {
    case ProjectionMode::Perspective: {
        proj.perspective(m_viewFactor, m_aspectRatio, m_nearPlane, m_farPlane);
        break;
    }
    case ProjectionMode::Orthogonal: {
        float half_width = m_viewFactor * m_aspectRatio * 0.5f;
        float half_height = m_viewFactor * 0.5f;
        proj.ortho(-half_width, half_width, -half_height, half_height, m_nearPlane, m_farPlane);
        break;
    }
    }
    return proj;
}

QMatrix4x4 Camera::view() const {
    QMatrix4x4 view;
    view.setToIdentity();
    view.rotate(m_orientation.conjugated());
    view.translate(-m_position);
    return view;
}
