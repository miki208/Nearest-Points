#ifndef GA05_QUICKHULL_H
#define GA05_QUICKHULL_H

#include "algorithms_practice/convexhull.h"

#define TOP_SIDE 1
#define BOTTOM_SIDE -1
#define NOT_FOUND -1

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
    std::vector<QPoint> _pointDrawVector;

    const QPoint *_pointToHighlight;
    bool _findingMaxPointIndicator;


    void _findHull(const QPoint &p1, const QPoint &p2, const std::vector<const QPoint*> &points);
    short _whichSide(const QPoint &p, const QPoint &p1, const QPoint &p2);
    double _calculateDistance(const QPoint &p, const QPoint &p1, const QPoint &p2);
    static bool _compare(const QPoint &p1, const QPoint &p2) { return p1.x() < p2.x(); }
};

#endif // GA05_QUICKHULL_H
