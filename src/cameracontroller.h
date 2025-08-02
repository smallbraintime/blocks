#pragma once

#include "camera.h"

class CameraController {
public:
    explicit CameraController(Camera* camera, const QVector3D& target, float radius)
        : m_camera(camera), m_target(target), m_radius(radius) { if (camera) updateCamera(); }

    void setCamera(Camera *camera) { m_camera = camera; }
    void setTarget(const QVector3D& target);
    void setRadius(float radius);
    void addYaw(float delta);
    void addPitch(float delta);
    void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

    const QVector3D& target() const { return m_target; }
    float radius() const { return m_radius; }
    float sensitivity() const { return m_sensitivity; }

private:
    Camera* m_camera{};
    QVector3D m_target;
    float m_radius{};
    float m_pitch{};
    float m_yaw{};
    float m_sensitivity = .05f;

    void updateCamera();
};

