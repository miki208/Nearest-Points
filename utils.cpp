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

    bool lineContainsPoint(QLine l, QPoint p)
    {
        //X must not be left from left end point
        if(l.p1().x() < l.p2().x())
        {
            if(p.x() < l.p1().x() || p.x() > l.p2().x())
                return false;
        }else{
            if(p.x() > l.p1().x() || p.x() < l.p2().x())
                return false;
        }

        if(l.p1().y() < l.p2().y()){
            if(p.y() < l.p1().y() || p.y() > l.p2().y())
                return false;
        }else{
            if(p.y() > l.p1().y() || p.y() < l.p2().y())
                return false;
        }

        return true;
    }

    bool lineIntersection(QLine l1, QLine l2, QPoint* i)
    {
        double k1 = static_cast<double>((l1.p1().y() - l1.p2().y()))/(l1.p1().x() - l1.p2().x());
        double k2 = static_cast<double>((l2.p1().y() - l2.p2().y()))/(l2.p1().x() - l2.p2().x());

        double n1 = l1.p1().y() - k1*l1.p1().x();
        double n2 = l2.p1().y() - k2*l2.p1().x();

        double dx = (n2-n1)/(k1-k2);
        int x = static_cast<int>(dx);
        int y = static_cast<int>(k1*dx + n1);

        *i = QPoint(x,y);

        return lineContainsPoint(l1, *i) && lineContainsPoint(l2, *i);
    }
}
