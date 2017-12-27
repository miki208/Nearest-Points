#ifndef GA19_CONVEXHULLFORDISKS_H
#define GA19_CONVEXHULLFORDISKS_H
#define PI 3.14159265


#include "algorithmbase.h"
#include <QLine>
#include <set>
#include <math.h>
namespace ConvexHullForDisks {


class Disk {
public:
    Disk(QPointF* center, double radius);
    QLineF* tangent(Disk* other_circle);
    QPointF* center;
    qreal radius;
};


class ConvexHullForDisks : public AlgorithmBase
{
public:
    ConvexHullForDisks(QWidget *pRender, int delayMs, std::string fileName);

    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();
    std::vector<Disk*> disks;
    std::vector<QLineF*> bridges;

};


class DiskSet {
public:
    std::set<Disk*> disk_set;
};

}
#endif // GA19_CONVEXHULLFORDISKS_H
