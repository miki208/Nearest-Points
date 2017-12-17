#ifndef GA00_INCREMENTALINSERTION_H
#define GA00_INCREMENTALINSERTION_H

#include "algorithms_practice/convexhull.h"

class Point;


//-----------------------------------------------------------------------------
// Incremental insertion
//-----------------------------------------------------------------------------
class IncrementalInsertion : public ConvexHull
{
public:
    IncrementalInsertion(QWidget *pRenderer, int delayMs, std::string filename = "", int inputSize = DEFAULT_POINTS_NUM);

    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

    std::vector<QPoint> getConvexHullTest() const;

private:
    int _xPositionOfSweepline;
    int _numberOfPointsInHull;
    bool _hasProblematicPoints;
    std::vector<Point> _complexPoints;

    bool _compare(const QPoint& p1, const QPoint& p2);
    bool _whichSide(QPoint x, QPoint x1, QPoint x2);
};

//-----------------------------------------------------------------------------
// Complex point
//-----------------------------------------------------------------------------
class Point
{
public:
    Point(const QPoint &value)
        : _value(value), _nextPoint(nullptr), _prevPoint(nullptr)
    {}

    Point(const Point &point)
        : _value(point._value), _nextPoint(point.getNextPoint()), _prevPoint(point.getPrevPoint())
    {}

    void setNextPoint(Point *point) { _nextPoint = point; }
    void setPrevPoint(Point *point) { _prevPoint = point; }
    Point* getNextPoint() const { return _nextPoint; }
    Point* getPrevPoint() const { return _prevPoint; }
    const QPoint& getValue() const { return _value; }

private:
    Point& operator=(const Point &point);

    const QPoint _value;
    Point *_nextPoint;
    Point *_prevPoint;
};

#endif // GA00_INCREMENTALINSERTION_H
