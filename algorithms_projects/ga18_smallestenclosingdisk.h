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


struct Disk
{
public:
    Disk(QPointF cen, double rad, double ar)
        : center(cen), r(rad), area(ar) {}
    Disk(QPointF p1, QPointF p2, QPointF p3);
    Disk(QPointF p1, QPointF p2);
    QPointF center;
    double r;
    double area;
};


class ga18_smallestEnclosingDisk : public AlgorithmBase
{
public:
    ga18_smallestEnclosingDisk(QWidget *pRenderer, int delayMs, std::string filename);

public:
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

private:
    int n;
    std::vector<QPointF> points;
    void minDisk( std::vector<QPointF> points );
    void minDisk( std::vector<QPointF> points, QPointF p1);
    void minDisk( std::vector<QPointF> points, QPointF p1, QPointF p2);
//    Circle circleEq( QPointF p1, QPointF p2, QPointF p3);
//    Circle circleEq( QPointF p1, QPointF p2);
};

#endif // GA18_SMALLESTENCLOSINGDISK_H
