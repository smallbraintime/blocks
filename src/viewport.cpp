#include "viewport.h"

QMatrix4x4 Viewport::projection() const {
    QMatrix4x4 projection;
    projection.setToIdentity();

    switch (m_mode) {
    case ProjectionMode::Perspective:
        projection.perspective(m_viewFactor, m_aspectRatio, m_nearPlane, m_farPlane);
        break;
    case ProjectionMode::Orthogonal:
        float half_width = m_viewFactor * m_aspectRatio * 0.5f;
        float half_height = m_viewFactor * 0.5f;
        projection.ortho(-half_width, half_width, -half_height, half_height, m_nearPlane, m_farPlane);
        break;
    }

    return projection;
}
