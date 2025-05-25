#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QSharedPointer>
#include <QStringView>
#include <QTimer>
#include <QObject>
#include <QHash>
#include <QWeakPointer>
#include <QVector>

#include "assetmanager.h"

class Entity;
class Camera;

class Scene : QObject {
    Q_OBJECT

public:
    struct SceneData {
        Camera* activeCamera = nullptr;
        QHash<QStringView, QSharedPointer<Entity>> entities;
    };

    explicit Scene(QObject* parent) : QObject(parent) {}

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void update(float deltaTime);
    QWeakPointer<Entity> createEntity(QStringView name);

    QWeakPointer<Entity> getEntity(QStringView name);

    void removeEntity(QStringView name);
    void setActiveCamera(Camera* camera) { m_sceneData.activeCamera = camera; }
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    const SceneData& getSceneData() const { return m_sceneData; }
    const AssetManager& getAssetManager() const { return m_assetManager; }

private:
    SceneData m_sceneData;
    AssetManager m_assetManager;
};
