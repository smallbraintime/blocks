#pragma once

#include <QMatrix4x4>

class Viewport
{
public:
    Viewport(float fov, float aspectRatio, float nearPlane, float farPlane)
        : m_fov(fov), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane) {}

    void setFov(float fov) { m_fov = fov; }
    void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
    void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
    void setfarPlane(float farPlane) { m_farPlane = farPlane; }

    QMatrix4x4 projection() const;
    float fov() const { return m_fov; }
    float aspectRatio() const { return m_aspectRatio; }
    float nearPlane() const { return m_nearPlane; }
    float farPlane() const { return m_farPlane; }

private:
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
};

