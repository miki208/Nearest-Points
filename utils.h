#ifndef COMMON_H
#define COMMON_H

#include <QPoint>

namespace utils
{
    bool negativeOrientation(QPoint p1, QPoint p2, QPoint p3);

    double distance(QPoint a, QPoint b);
}

#endif // COMMON_H
