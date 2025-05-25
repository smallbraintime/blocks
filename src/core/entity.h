#pragma once

#include <optional>

#include "scene.h"
#include "graphicscomponents.h"
#include "camera.h"

class Entity {
public:
    Entity(Scene& scene, int id) : m_scene(scene) {}

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    Scene& getScene() { return m_scene; }

    std::optional<Camera> camera;
    std::optional<StaticMesh> staticMesh;
    std::optional<InstancedStaticMesh> instancedStaticMesh;
    std::optional<Light> light;

    virtual void onUpdate(float deltaTime);
    virtual void keyPressEvent(QKeyEvent* event) {}
    virtual void keyReleaseEvent(QKeyEvent* event) {}
    virtual void mouseMoveEvent(QMouseEvent* event) {}

private:
    Scene& m_scene;
};
