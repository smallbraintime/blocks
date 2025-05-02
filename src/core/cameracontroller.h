#pragma once

#include <QSharedPointer>

#include "camera.h"

class CameraController
{
public:
    explicit CameraController(QSharedPointer<Camera> camera) : m_camera(std::move(camera)) {}
    explicit CameraController(QSharedPointer<Camera> camera, float speed, float sentisivity)
        : m_camera(std::move(camera)), m_speed(speed), m_sentisivity(sentisivity) {}

    void moveForward(float axis);
    void moveRight(float axis);
    void pitch(float angle);
    void yaw(float angle);
    void setSpeed(float speed) { m_speed = speed; }
    void setSentisivity(float sentisivity) { m_sentisivity = sentisivity; }

    float speed() const { return m_speed; }
    float sentisivity() const { return m_sentisivity; }

private:
    QSharedPointer<Camera> m_camera;
    float m_speed;
    float m_sentisivity;
    float m_pitch;
    float m_yaw;
};

