#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QSharedPointer>
#include <QStringView>

#include "graphicscomponent.h"
#include "camera.h"
#include "scene.h"

class Entity {
public:
    Entity(QStringView id, Scene* scene) : m_id(id), m_scene(scene) {}

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    template<typename T, char* id, typename... Args>
    QSharedPointer<T> addComponent(Args&&... args) {

    }

    void setActiveCamera(QSharedPointer<Camera> camera) { m_scene->setActiveCamera(std::move(camera)); }

    virtual void onUpdate(float deltaTime) {};
    virtual void keyPressEvent(QKeyEvent* event) {};
    virtual void keyReleaseEvent(QKeyEvent* event) {};
    virtual void mouseMoveEvent(QMouseEvent* event) {};
    virtual ~Entity() = default;

private:
    QStringView m_id;
    bool m_valid = true;
    QMap<QStringView, QSharedPointer<GraphicsComponent>> m_components;
    Scene* m_scene;
};
