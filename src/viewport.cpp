#include "viewport.h"

QMatrix4x4 Viewport::projection() const {
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
    return projection;
}
