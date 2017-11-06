#include "utils.h"

namespace utils
{
    bool negativeOrientation(QPoint p1, QPoint p2, QPoint p3)
    {
        return ((p2.x() - p1.x())*(p3.y() - p1.y()) - (p3.x() - p1.x())*(p2.y() - p1.y())) <= 0;
    }

    double distance(QPoint a, QPoint b)
    {
        if((a.x() == b.x()) && (a.y() == b.y())) return 0;
        return sqrt(pow((a.x() - b.x()),2) + pow(a.y() - b.y() ,2));
    }
}
