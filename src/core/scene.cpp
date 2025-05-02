#include "scene.h"

#include <QDateTime>

#include "entity.h"

void Scene::update(float deltaTime) {
    for (const auto& entity : m_sceneData.m_entities) {
        entity->onUpdate(deltaTime);
    }
}

void Scene::keyPressEvent(QKeyEvent* event) {
    for (const auto& entity : m_sceneData.m_entities) {
        entity->keyPressEvent(event);
    }
}

void Scene::keyReleaseEvent(QKeyEvent* event) {
    for (const auto& entity : m_sceneData.m_entities) {
        entity->keyReleaseEvent(event);
    }
}

void Scene::mouseMoveEvent(QMouseEvent* event) {
    for (const auto& entity : m_sceneData.m_entities) {
        entity->mouseMoveEvent(event);
    }
}
