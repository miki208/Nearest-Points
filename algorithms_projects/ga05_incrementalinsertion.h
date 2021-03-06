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
    std::vector<QPoint> getConvexHull() const;

private:
    unsigned int _xPositionOfSweepline;
    unsigned int _numberOfProcessedPoints;

    bool _initialCollinearPointsIndicator;
    Point *_firstCollinearPoint;

    std::vector<Point> _complexPoints;
    std::vector<QPoint> _convexHull;

    bool _compare(const QPoint& p1, const QPoint& p2) const;
    bool _whichSide(const QPoint p, const QPoint p1, const QPoint p2) const;
    Point* _checkCollinearity(Point &x1, Point &x2, Point &x3) const;
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
