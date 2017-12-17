#ifndef GA02_CONVEXHULL_H
#define GA02_CONVEXHULL_H

#include "algorithmbase.h"

#include <set>
#include <functional>

class ConvexHull : public AlgorithmBase
{
public:
    ConvexHull(QWidget *pRenderer, int delayMs, std::string filename = "", int inputSize = DEFAULT_POINTS_NUM);

public:
    // AlgorithmBase interface
    /*Optimalni algoritam za izracunavanje konveksnog omotaca - Graham scan - O(nlogn)*/
    void runAlgorithm();

    void drawAlgorithm(QPainter &painter) const;

    /* Algoritam grube sile O(n^3)*/
    void runNaiveAlgorithm();

    std::vector<QPoint> convexHull() const;

    std::vector<QPoint> convexHullTest() const;

private:
    /* Funckija poredjenja za std::sort */
    bool compare(const QPoint& p1, const QPoint& p2);


private:
     /*Ulazni podaci*/
     std::vector<QPoint> _points;

     /* Najdesnija tacka*/
     QPoint _firstPoint;

     /* Konveksni omotac izracunat optimalnim algoritmom */
     std::vector<QPoint> _convexHull;

     /* Konveksni omotac izracunat algoritmom grube sile */
     std::vector<QPoint> _convexHullTest;
};

#endif // GA02_CONVEXHULL_H
