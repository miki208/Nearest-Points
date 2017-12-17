#ifndef GA05_QUICKHULL_H
#define GA05_QUICKHULL_H

#include "algorithms_practice/convexhull.h"

class QuickHull : public ConvexHull
{
public:
    QuickHull(QWidget *pRenderer, int delayMs, std::string filename = "", int inputSize = DEFAULT_POINTS_NUM);

    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

    std::vector<QPoint> getConvexHullTest() const;
    std::vector<QPoint> getConvexHull() const;

private:
    std::vector<QPoint> _convexHull;

    bool _whichSide(QPoint x, QPoint x1, QPoint x2);
};

#endif // GA05_QUICKHULL_H
