#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QStringView>
#include <QWidget>
#include <QTimer>
#include <QObject>
#include <QHash>

#include "camera.h"
#include "assetmanager.h"

class Entity;

struct SceneData {
    QHash<QStringView, QSharedPointer<Entity>> m_entities; // not really cache friendly
    QSharedPointer<Camera> m_activeCamera;
};

class Scene : QObject {
    Q_OBJECT

public:
    explicit Scene(QObject* parent) : QObject(parent) {}

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void addEntity(QStringView name, QSharedPointer<Entity> entity);
    void setActiveCamera(QSharedPointer<Camera> camera) { m_sceneData.m_activeCamera = std::move(camera); }
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    template<typename T>
    QSharedPointer<T> getEntity() {
        // qHash("player");
    }

    const SceneData& getSceneData() const { return m_sceneData; }
    const AssetManager& getAssetManager() const { return m_assetManager; }

public slots:
    void update(float deltaTime);

private:
    SceneData m_sceneData;
    AssetManager m_assetManager;
};
