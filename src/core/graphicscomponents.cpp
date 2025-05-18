#include "graphicscomponents.h"

void StaticMesh::beginFrame(QOpenGLShaderProgram &program) {
    m_mesh->beginFrame(program);
    m_material->beginFrame(program);

    program.setUniformValue("uModel", m_transform);
}

void StaticMesh::endFrame(QOpenGLShaderProgram &program) {
    m_mesh->endFrame(program);
    m_material->endFrame(program);
}

InstancedStaticMesh::~InstancedStaticMesh() {
    m_vbo.create();
    if (!m_vbo.isCreated()) {
        qWarning("Failed to create Vbo.");
        return;
    }
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
}

void InstancedStaticMesh::addInstance(const QMatrix4x4 &position, const QColor &color) {
    m_instances.append({position, color});
    m_isModified = true;
}

void InstancedStaticMesh::removeInstanceAt(size_t index) {
    m_instances.removeAt(index);
    m_isModified = true;
}

void InstancedStaticMesh::setPositionAt(size_t index, const QMatrix4x4 &transform) {
    m_instances[index].m_transform = transform;
    m_isModified = true;
}

void InstancedStaticMesh::setColorAt(size_t index, const QColor &color) {
    m_instances[index].m_color = color;
    m_isModified = true;
}

void InstancedStaticMesh::beginFrame(QOpenGLShaderProgram &program) {
    m_mesh->beginFrame(program);

    if (m_isTransparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    }

    if (m_isModified) {
        m_vbo.allocate(m_instances.data(), m_instances.size() * sizeof(StaticMeshInstance));
        m_isModified = false;
    }
    m_vbo.bind();
}

void InstancedStaticMesh::endFrame(QOpenGLShaderProgram &program) {
    m_mesh->endFrame(program);

    if (m_isTransparent) {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }

    m_vbo.release();
}

void Light::beginFrame(QOpenGLShaderProgram &program) {
    program.setUniformValue("uType", (char)type());
    program.setUniformValue("uPosition", position());
    program.setUniformValue("uDirection", direction());
    program.setUniformValue("uColor", color());
    program.setUniformValue("uIntensity", intensity());
}

void Light::endFrame(QOpenGLShaderProgram &program) {}
