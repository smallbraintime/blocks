#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QStringView>
#include <QSet>

#include "camera.h"
#include "graphicscomponent.h"

class Entity;

struct SceneData {
    QMap<QStringView, QWeakPointer<GraphicsComponent>> m_components; // not really cache friendly
    QSharedPointer<Camera> m_activeCamera;
};

class Scene {
public:
    Scene() {}

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void addEntity(QStringView id, QSharedPointer<Entity> entity);
    void update(float deltaTime);
    void setActiveCamera(QSharedPointer<Camera> camera) { m_sceneData->m_activeCamera = std::move(camera); }
    template<typename T>
    QSharedPointer<T> getEntity(QStringView id);

    const SceneData& getSceneData() { return *m_sceneData; }

private:
    QMap<QStringView, QSharedPointer<Entity>> m_entities;
    QSharedPointer<SceneData> m_sceneData;
};
