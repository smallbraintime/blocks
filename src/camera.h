#pragma once

#include <QVector3D>
#include <QMatrix4x4>

class Camera {
public:
    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const;

    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);

    void processMouseMovement(float xOffset, float yOffset);
    void processMouseScroll(float yOffset);

private:
    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;
    QVector3D worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float sensitivity;
    float fov;
};
