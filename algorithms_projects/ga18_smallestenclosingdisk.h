#ifndef GA18_SMALLESTENCLOSINGDISK_H
#define GA18_SMALLESTENCLOSINGDISK_H

#include "algorithmbase.h"
#include "utils.h"
#include <QPointF>
#include <QPoint>
#include <vector>

const double pi__ = 3.1415926535898;

namespace my_utils
{

    double distance(QPointF a, QPointF b);

}


struct Circle
{
public:
    Circle(QPointF cen, double rad, double ar)
        : center(cen), r(rad), area(ar) {}
    Circle( const Circle& another)
        : center(another.center), r(another.r), area(another.area) {}
    Circle& operator=( const Circle other );
    Circle(QPointF p1, QPointF p2, QPointF p3);
    Circle(QPointF p1, QPointF p2);
public:
    QPointF center;
    double r;
    double area;
};


class MinEnclosingCircle : public AlgorithmBase
{
public:
    MinEnclosingCircle(QWidget *pRenderer, int delayMs, std::string filename);
    MinEnclosingCircle(QWidget *pRenderer, int delayMs, std::vector<QPointF> newPoints );

public:
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;

/*  BRUTE FORCE:
    - Theorem: For any finite set of points in
    general position, the smallest enclosing
    disk either has at least three points on its
    boundary, or two points which form a
    diameter. If there are three points, they
    subdivide the circle into three arcs of
    length no more than PI each. Prove !

    - This implies a O(n^4) algorithm
*/
    void runNaiveAlgorithm();
    Circle result() const;
    void clearResult();

private:
    int n;
    std::vector<QPointF> points;
    Circle minCircle;
    void minDisk( std::vector<QPointF> points );
    void minDisk( std::vector<QPointF> points, QPointF p1);
    void minDisk( std::vector<QPointF> points, QPointF p1, QPointF p2);
    bool circleEnclosesPoints(Circle currCircle);
//    Circle circleEq( QPointF p1, QPointF p2, QPointF p3);
//    Circle circleEq( QPointF p1, QPointF p2);
};

#endif // GA18_SMALLESTENCLOSINGDISK_H
