#ifndef COMMON_H
#define COMMON_H

#include <QPoint>
#include <QLine>
namespace utils
{
    bool negativeOrientation(QPoint p1, QPoint p2, QPoint p3);

    double distance(QPoint a, QPoint b);

    bool lineIntersection(QLineF l1, QLineF l2, QPointF* i);

    bool lineContainsPoint(QLineF l, QPointF p);
}

#endif // COMMON_H
