#include <QPainter>
#include <QDebug>
#include <QtMath>

#include "ga05_quickhull.h"
#include "utils.h"


//-----------------------------------------------------------------------------
// QuickHull
//-----------------------------------------------------------------------------
QuickHull::QuickHull(QWidget *pRenderer, int delayMs, std::string filename, int inputSize)
    : ConvexHull(pRenderer, delayMs, filename, inputSize),
      _findingMaxPointIndicator(false)
{}

//-----------------------------------------------------------------------------
// _findHull
//-----------------------------------------------------------------------------
void QuickHull::_findHull(const QPoint &p1, const QPoint &p2, const std::vector<const QPoint*> &points)
{
    double maxDistance = 0.0;
    double currentDistance = 0.0;
    int maxPointIndex = NOT_FOUND;

    _findingMaxPointIndicator = true;
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        _pointToHighlight = points[i];
        currentDistance = _calculateDistance(*points[i], p1, p2);

        if (maxDistance < currentDistance)
        {
            maxDistance = currentDistance;
            maxPointIndex = i;
        }

        AlgorithmBase_updateCanvasAndBlock();
    }
    _findingMaxPointIndicator = false;

    if (maxPointIndex == NOT_FOUND)
        return;

    _convexHull.push_back(*points[maxPointIndex]);

    _pointDrawVector.push_back(p1);
    _pointDrawVector.push_back(*points[maxPointIndex]);
    AlgorithmBase_updateCanvasAndBlock();
    _pointDrawVector.push_back(*points[maxPointIndex]);
    _pointDrawVector.push_back(p2);
    AlgorithmBase_updateCanvasAndBlock();

    std::vector<const QPoint*> topSidePoints;
    std::vector<const QPoint*> bottomSidePoints;
    for (const QPoint *p : points)
    {
        if (_whichSide(*p, p1, *points[maxPointIndex]) == TOP_SIDE)
            topSidePoints.push_back(p);

        if (_whichSide(*p, p2, *points[maxPointIndex]) == BOTTOM_SIDE)
            bottomSidePoints.push_back(p);
    }

    _findHull(p1, *points[maxPointIndex], topSidePoints);
    _findHull(*points[maxPointIndex], p2, bottomSidePoints);
}

//-----------------------------------------------------------------------------
// runAlgorithm
//-----------------------------------------------------------------------------
void QuickHull::runAlgorithm()
{
    // find leftmost point
    std::vector<QPoint>::iterator minXPoint = std::min_element(_points.begin(), _points.end(), _compare);
    // find rightmost point
    std::vector<QPoint>::iterator maxXPoint = std::max_element(_points.begin(), _points.end(), _compare);

    // add minXPoint and maxXPoint to convex hull
    _convexHull.push_back(*minXPoint);
    _convexHull.push_back(*maxXPoint);
    _pointDrawVector.push_back(*minXPoint);
    _pointDrawVector.push_back(*maxXPoint);

    std::vector<const QPoint*> topSidePoints;
    std::vector<const QPoint*> bottomSidePoints;
    for (const QPoint &p : _points)
    {
        if (_whichSide(p, *minXPoint, *maxXPoint) == TOP_SIDE)
            topSidePoints.push_back(&p);
        else if (_whichSide(p, *minXPoint, *maxXPoint) == BOTTOM_SIDE)
            bottomSidePoints.push_back(&p);
    }

    _findHull(*minXPoint, *maxXPoint, topSidePoints);
    _findHull(*maxXPoint, *minXPoint, bottomSidePoints);

    emit animationFinished();
}

//-----------------------------------------------------------------------------
// drawAlgorithm
//-----------------------------------------------------------------------------
void QuickHull::drawAlgorithm(QPainter &painter) const
{
    // set painter
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen p = painter.pen();

    p.setWidth(2);
    p.setColor(Qt::darkBlue);
    painter.setPen(p);
    for (int i = 0; i < (_pointDrawVector.size()/2); i++)
    {
        painter.drawLine(_pointDrawVector[2*i], _pointDrawVector[2*i+1]);
    }

    // draw points
    p.setWidth(5);
    p.setCapStyle(Qt::RoundCap);
    p.setColor(Qt::darkRed);
    painter.setPen(p);
    for (const QPoint &pt : _points)
    {
        painter.drawPoint(pt);
    }

    if (_findingMaxPointIndicator == true)
    {
        p.setWidth(10);
        p.setCapStyle(Qt::RoundCap);
        p.setColor(Qt::red);
        painter.setPen(p);
        painter.drawPoint(_pointToHighlight->x(), _pointToHighlight->y());
    }
}

//-----------------------------------------------------------------------------
// runNaiveAlgorithm
//-----------------------------------------------------------------------------
void QuickHull::runNaiveAlgorithm()
{
    ConvexHull::runNaiveAlgorithm();
}

//-----------------------------------------------------------------------------
// _calculateDistance
//-----------------------------------------------------------------------------
double QuickHull::_calculateDistance(const QPoint &p, const QPoint &p1, const QPoint &p2)
{
    double nominator = (p2.y() - p1.y())*p.x() - (p2.x() - p1.x())*p.y() + p2.x()*p1.y() - p2.y()*p1.x();
    double denominator = qPow((p2.y() - p1.y()), 2) + qPow((p2.x() - p1.x()), 2);

    return abs(nominator)/sqrt(denominator);
}

//-----------------------------------------------------------------------------
// _whichSide
//-----------------------------------------------------------------------------
short QuickHull::_whichSide(const QPoint &p, const QPoint &p1, const QPoint &p2)
{
    int sgn = (p.x() - p1.x())*(p2.y()-p1.y()) - (p.y() - p1.y())*(p2.x() - p1.x());

    if (sgn > 0)
    {
        return 1;
    }
    else if (sgn < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------
// getConvexHull
//-----------------------------------------------------------------------------
std::vector<QPoint> QuickHull::getConvexHull() const
{
    return _convexHull;
}

//-----------------------------------------------------------------------------
// getConvexHullTest
//-----------------------------------------------------------------------------
std::vector<QPoint> QuickHull::getConvexHullTest() const
{
    return ConvexHull::convexHullTest();
}
