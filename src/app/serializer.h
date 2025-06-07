#pragma once

#include <QVector>
#include <QColor>
#include <QString>

class BlkSerializer {
public:
    static void serialize(const QString& filepath, const QVector<QColor>& blocks);
    static QVector<QColor>& deserialize(const QString& filepath);
};

void serializeToObj(const QString& filepath, const QVector<QColor>& blocks);
