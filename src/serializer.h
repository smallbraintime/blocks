#pragma once

#include "block.h"

class BlkSerializer
{
public:
    static void serialize(const QVector<Block>& blocks, const QString& filepath);
    static QVector<Block> deserialize(const QString& filepath);
};

class ObjSerializer {
public:
    static void serialize(const QVector<Block>& blocks, const QString& filepath);
};
