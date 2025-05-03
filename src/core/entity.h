#pragma once

#include "scene.h"
#include "graphicscomponent.h"

class Entity {
public:
    Entity(Scene* scene, size_t id) : m_scene(scene), m_id(id) {}

    template <typename T, typename... Args>
    T& addComponent() {}
    template <typename T, typename... Args>
    T& getComponent() {}
    template <typename T, typename... Args>
    void removeComponent() {}
    virtual void onUpdate(float deltaTime);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    size_t m_id;
    std::vector<GraphicsComponent> m_components;
    Scene* m_scene;
};
