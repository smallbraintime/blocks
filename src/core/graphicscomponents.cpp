#include "graphicscomponents.h"

InstancedStaticMesh::~InstancedStaticMesh() {
    m_ssbo.create();
    if (!m_ssbo.isCreated()) {
        qWarning("Failed to create Vbo.");
        return;
    }
}

void InstancedStaticMesh::addInstance(const QMatrix4x4 &position, const QColor &color) {
    m_instances.append({position, color});
    m_isModified = true;
}

void InstancedStaticMesh::removeInstanceAt(size_t index) {
    m_instances.removeAt(index);
    m_isModified = true;
}

void InstancedStaticMesh::setTransformAt(size_t index, const QMatrix4x4 &transform) {
    m_instances[index].m_transform = transform;
    m_isModified = true;
}

void InstancedStaticMesh::setColorAt(size_t index, const QColor &color) {
    m_instances[index].m_color = color;
    m_isModified = true;
}
