#include "cameracontroller.h"

void CameraController::moveForward(float axis) {
    m_camera->translate({0.0f, 0.0f, -m_speed * axis});
}

void CameraController::moveRight(float axis) {
    m_camera->translate({m_speed * axis, 0.0f, 0.0f});
}

void CameraController::moveUp(float axis) {
    m_camera->translate({0.0f, m_speed * axis, 0.0f});
}

void CameraController::pitch(float angle) {
    m_pitch += angle * m_sentisivity;

    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    m_camera->set_orientation({m_pitch, m_yaw, 0.0f});
}

void CameraController::yaw(float angle) {
    m_yaw += angle * m_sentisivity;

    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    m_camera->set_orientation({m_pitch, m_yaw, 0.0f});
}
