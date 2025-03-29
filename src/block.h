#pragma once

#include <QColor>
#include <QVector>

struct Block {
    QVector<int> pos[3];
    QColor color;
};
