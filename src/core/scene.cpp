#include "scene.h"

void Scene::update(float deltaTime) {
    for (auto& obj : m_objects) {
        obj->onUpdate(deltaTime);
    }
}
