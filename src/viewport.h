#pragma once

#include <QMatrix4x4>

class Viewport
{
public:
    enum class ProjectionMode {
        Perspective,
        Orthogonal,
    };

    Viewport(ProjectionMode mode, float viewFactor, float aspectRatio, float nearPlane, float farPlane)
        : m_mode(mode), m_viewFactor(viewFactor), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane) {
    }

    Viewport(float fov, float aspectRatio, float nearPlane, float farPlane)
        : m_viewFactor(fov), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane), m_mode(ProjectionMode::Perspective) {}


    void setFov(float fov) { m_viewFactor = fov; }
    void setSize(float size) { m_viewFactor = size; }
    void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }
    void setNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
    void setfarPlane(float farPlane) { m_farPlane = farPlane; }
    void setProjectionMode(ProjectionMode mode) { m_mode = mode; }

    QMatrix4x4 projection() const;
    float fov() const { return m_viewFactor; }
    float size() const { return m_viewFactor; }
    float aspectRatio() const { return m_aspectRatio; }
    float nearPlane() const { return m_nearPlane; }
    float farPlane() const { return m_farPlane; }

private:
    float m_viewFactor; // can be treated as a fov or size depends on camera mode
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
    ProjectionMode m_mode;
};
