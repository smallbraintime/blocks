#pragma once

#include <QVector>

#include "block.h"

class ObjSerializer {
public:
    static void serialize(const QVector<Block>& blocks, const QString& filepath);
    static QVector<Block> deserialize(const QString& filepath);
};
