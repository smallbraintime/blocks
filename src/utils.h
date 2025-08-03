#pragma once

#include <QColor>

class Color {
public:
    int r, g, b, a;
    Color& operator=(const QColor& qcolor) {
        r = qcolor.red();
        g = qcolor.green();
        b = qcolor.blue();
        a = qcolor.alpha();
        return *this;
    }
};
