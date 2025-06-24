#pragma once

#include "camera.h"

class CameraController {
public:
    explicit CameraController(Camera* camera, float speed, float sentisivity)
        : m_camera(camera), m_speed(speed), m_sentisivity(sentisivity) {}

    void moveForward(float axis);
    void moveRight(float axis);
    void moveUp(float axis);
    void pitch(float angle);
    void yaw(float angle);
    void setSpeed(float speed) { m_speed = speed; }
    void setSentisivity(float sentisivity) { m_sentisivity = sentisivity; }

    float speed() const { return m_speed; }
    float sentisivity() const { return m_sentisivity; }

private:
    Camera* m_camera;
    float m_speed;
    float m_sentisivity;
    float m_pitch;
    float m_yaw;
};

