#pragma once

#include <QVector>
#include <QHash>
#include <memory>

#include "scene.h"
#include "graphicscomponent.h"

class Entity {
public:
    using TypeId = size_t;

    Entity(Scene& scene, size_t id) : m_scene(scene) {}

    template <typename T, typename... Args>
    T& addComponent(Args&&... args) {
        if constexpr (typeid(T) == typeid(Camera)) {
            m_camera = std::make_unique(std::forward<Args>(args)...);
            return m_camera;
        }

        return m_components.insert(typeid(T).hash_code(), T(std::forward<Args>(args)...));
    }

    template <typename T>
    T& getComponent() {
        if constexpr (typeid(T) == typeid(Camera)) {
            return m_camera;
        }

        return static_cast<T>(m_components.find(typeid(T).hash_code()));
    }

    template <typename T>
    void removeComponent() {
        if constexpr (typeid(T) == typeid(Camera)) {
            m_camera.release();
        } else {
            m_components.remove(typeid(T).hash_code());
        }
    }

    template <typename T>
    bool hasComponent() const {
        if constexpr (typeid(T) == typeid(Camera)) {
            return m_camera != nullptr;
        }

        return m_components.contains(typeid(T).hash_code());
    }

    Scene& getScene() { return m_scene; }

    const QHash<TypeId, GraphicsComponent>& getComponents() { return m_components; }

    virtual void onUpdate(float deltaTime);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    QHash<TypeId, GraphicsComponent> m_components;
    std::unique_ptr<Camera> m_camera = nullptr;
    Scene& m_scene;
};
