#include "scene.h"

#include <QDateTime>

#include "entity.h"

void Scene::update(float deltaTime) {
    for (const auto& entity : m_sceneData.m_entities) {
        entity->onUpdate(deltaTime);
    }
}

QWeakPointer<Entity> Scene::createEntity(QStringView name) {
    size_t id = qHash(name); // can we ommit hashing this two times
    QSharedPointer<Entity> newEntity(new Entity(*this, id));
    m_sceneData.m_entities.insert(name, newEntity);
    return newEntity;
}

QWeakPointer<Entity> Scene::getEntity(QStringView name) {
    return m_sceneData.m_entities.find(name).value();
}

void Scene::removeEntity(QStringView name) {
    m_sceneData.m_entities.remove(name);
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
