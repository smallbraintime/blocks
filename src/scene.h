#pragma once

#include <QColor>
#include <QVector>
#include <QSharedPointer>

#include "camera.h"

#define CHUNK_SIZE 1000

struct Scene {
    QVector<QColor> blocks{CHUNK_SIZE};
    QSharedPointer<Camera> camera;
};
