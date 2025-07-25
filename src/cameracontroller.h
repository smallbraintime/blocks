#pragma once

#include "camera.h"

class CameraController {
public:
    explicit CameraController(Camera* camera, float speed, float sentisivity, const QVector3D& target, float radius)
        : m_camera(camera), m_speed(speed), m_sensitivity(sentisivity), m_target(target), m_radius(radius) {}

    void setCamera(Camera *camera) { m_camera = camera; }
    void setPosition(const QVector3D& position) { if (!m_camera) m_camera->setPosition(position); };
    void setSpeed(float speed) { m_speed = speed; }
    void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }
    void setRadius(float radius) { m_radius = radius; }
    void setTarget(const QVector3D& target) { m_target = target; }
    void addYaw(float delta);
    void addPitch(float delta);

    float speed() const { return m_speed; }
    float sensitivity() const { return m_sensitivity; }
    float radius() const { return m_radius; }

private:
    Camera* m_camera;
    float m_speed;
    float m_sensitivity;
    QVector3D m_target;
    float m_radius;
    float m_pitch;
    float m_yaw;

    void updateCamera();
};

