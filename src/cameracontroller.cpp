#include "cameracontroller.h"

void CameraController::setRadius(float radius) {
    m_radius = radius;
    if (m_camera) updateCamera();
}

void CameraController::setTarget(const QVector3D &target) {
    m_target = target;
    if (m_camera) updateCamera();
}

void CameraController::addYaw(float delta) {
    m_yaw += delta * m_sensitivity;
    updateCamera();
}

void CameraController::addPitch(float delta) {
    m_pitch += delta * m_sensitivity;
    m_pitch = qBound(-89.0f, m_pitch, 89.0f);
    updateCamera();
}

void CameraController::updateCamera() {
    if (!m_camera) return;

    float yawRad = qDegreesToRadians(m_yaw);
    float pitchRad = qDegreesToRadians(m_pitch);

    QVector3D offset;
    offset.setX(m_radius * qCos(pitchRad) * qSin(yawRad));
    offset.setY(m_radius * qSin(pitchRad));
    offset.setZ(m_radius * qCos(pitchRad) * qCos(yawRad));

    QVector3D position = m_target + offset;
    m_camera->setPosition(position);
    m_camera->lookAt(m_target);
}
