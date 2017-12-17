#ifndef GA02_CONVEXHULL_H
#define GA02_CONVEXHULL_H

#include "convexhull.h"

#include <set>
#include <functional>

class GrahamScan : public ConvexHull
{
public:
    GrahamScan(QWidget *pRenderer, int delayMs, std::string filename = "", int inputSize = DEFAULT_POINTS_NUM);

public:
    /*Optimalni algoritam za izracunavanje konveksnog omotaca - Graham scan - O(nlogn)*/
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

    std::vector<QPoint> convexHullTest() const;
    std::vector<QPoint> convexHull() const;

private:
    /* Funckija poredjenja za std::sort */
    bool compare(const QPoint& p1, const QPoint& p2);

    /* Najdesnija tacka */
    QPoint _firstPoint;

    /* Konveksni omotac izracunat optimalnim algoritmom */
    std::vector<QPoint> _convexHull;
};

#endif // GA02_CONVEXHULL_H
