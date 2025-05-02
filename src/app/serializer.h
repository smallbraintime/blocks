#pragma once

#include <QVector>

#include "voxel.h"

class BlkSerializer
{
public:
    static void serialize(const QVector<Voxel>& blocks, const QString& filepath);
    static QVector<Voxel> deserialize(const QString& filepath);
};

void serializeToObj(const QVector<Voxel>& blocks, const QString& filepath);
