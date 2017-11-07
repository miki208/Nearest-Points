#ifndef COMMON_H
#define COMMON_H

#include <QPoint>
#include <QLine>
namespace utils
{
    bool negativeOrientation(QPoint p1, QPoint p2, QPoint p3);

    double distance(QPoint a, QPoint b);

    bool lineIntersection(QLine l1, QLine l2, QPoint* i);

    bool lineContainsPoint(QLine l, QPoint p);
}

#endif // COMMON_H
