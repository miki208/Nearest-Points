#include <QPainter>
#include <QDebug>
#include "ga05_incrementalinsertion.h"
#include "utils.h"


//-----------------------------------------------------------------------------
// Incremental insertion
//-----------------------------------------------------------------------------
IncrementalInsertion::IncrementalInsertion(QWidget *pRenderer, int delayMs, std::string filename, int inputSize)
    : ConvexHull(pRenderer, delayMs, filename, inputSize),
      _xPositionOfSweepline(0), _numberOfProcessedPoints(0), _initialCollinearPointsIndicator(true), _firstCollinearPoint(nullptr)
{}

//-----------------------------------------------------------------------------
// runAlgorithm
//-----------------------------------------------------------------------------
void IncrementalInsertion::runAlgorithm()
{
    // remove duplicates
    auto it = std::unique(_points.begin(), _points.end());
    _points.resize(std::distance(_points.begin(), it));

    // check number of points
    if (_points.size() < 3)
        return;

    // sort raw points by their x-coordinates
    std::sort(_points.begin(), _points.end(), [&](const QPoint& lhs, const QPoint& rhs){ return _compare(lhs, rhs); });

    // create complex points
    for (const QPoint &p : _points)
        _complexPoints.push_back(Point(p));

    // show points on the canvas
    AlgorithmBase_updateCanvasAndBlock();

    // init algorithm
    _complexPoints[0].setNextPoint(&_complexPoints[1]);
    _complexPoints[0].setPrevPoint(&_complexPoints[1]);
    _complexPoints[1].setNextPoint(&_complexPoints[0]);
    _complexPoints[1].setPrevPoint(&_complexPoints[0]);

    // process the 1st point
    ++_numberOfProcessedPoints;
    _xPositionOfSweepline = _complexPoints[0].getValue().x();
    AlgorithmBase_updateCanvasAndBlock();

    // process the 2nd point
    ++_numberOfProcessedPoints;
    _xPositionOfSweepline = _complexPoints[1].getValue().x();
    AlgorithmBase_updateCanvasAndBlock();

    // process the remaining points
    for (unsigned int i = 2; i < _complexPoints.size(); ++i)
    {
        // move the sweep line and increment number of processed points
        ++_numberOfProcessedPoints;
        _xPositionOfSweepline = _complexPoints[i].getValue().x();

        // check and process initial collinear points
        if (_initialCollinearPointsIndicator == true)
        {
            // check and save the first collinear point
            if (_firstCollinearPoint == nullptr)
                _firstCollinearPoint = _checkCollinearity(_complexPoints[i-2], _complexPoints[i-1], _complexPoints[i]);

            if (_firstCollinearPoint != nullptr)
            {
                if (_checkCollinearity(_complexPoints[i-2], _complexPoints[i-1], _complexPoints[i]) != nullptr)
                {
                    _complexPoints[i].setNextPoint(_firstCollinearPoint);
                    _complexPoints[i].setPrevPoint(&_complexPoints[i-1]);
                    _firstCollinearPoint->setPrevPoint(&_complexPoints[i]);
                    _complexPoints[i-1].setNextPoint(&_complexPoints[i]);

                    AlgorithmBase_updateCanvasAndBlock();
                    continue;
                }
                else
                {
                    _initialCollinearPointsIndicator = false;
                }
            }
            else
            {
                _initialCollinearPointsIndicator = false;
            }
        }

        // process other non-collinear points
        if (_complexPoints[i].getValue().y() < _complexPoints[i-1].getValue().y())    // curPoint is above prevPoint
        {
            if (_whichSide(_complexPoints[i].getValue(), _complexPoints[i-1].getValue(), _complexPoints[i-1].getPrevPoint()->getValue()))
            {
                _complexPoints[i].setNextPoint(&_complexPoints[i-1]);
                _complexPoints[i].setPrevPoint(_complexPoints[i-1].getPrevPoint());
                _complexPoints[i-1].getPrevPoint()->setNextPoint(&_complexPoints[i]);
                _complexPoints[i-1].setPrevPoint(&_complexPoints[i]);
            }
            else
            {
                _complexPoints[i].setNextPoint(_complexPoints[i-1].getNextPoint());
                _complexPoints[i].setPrevPoint(&_complexPoints[i-1]);
                _complexPoints[i-1].getNextPoint()->setPrevPoint(&_complexPoints[i]);
                _complexPoints[i-1].setNextPoint(&_complexPoints[i]);
            }
        }
        else // curPoint is below prevPoint
        {
            if (_whichSide(_complexPoints[i].getValue(), _complexPoints[i-1].getValue(), _complexPoints[i-1].getPrevPoint()->getValue()))
            {
                _complexPoints[i].setNextPoint(&_complexPoints[i-1]);
                _complexPoints[i].setPrevPoint(_complexPoints[i-1].getPrevPoint());
                _complexPoints[i-1].getPrevPoint()->setNextPoint(&_complexPoints[i]);
                _complexPoints[i-1].setPrevPoint(&_complexPoints[i]);
            }
            else
            {
                _complexPoints[i].setNextPoint(_complexPoints[i-1].getNextPoint());
                _complexPoints[i].setPrevPoint(&_complexPoints[i-1]);
                _complexPoints[i-1].getNextPoint()->setPrevPoint(&_complexPoints[i]);
                _complexPoints[i-1].setNextPoint(&_complexPoints[i]);
            }
        }
        AlgorithmBase_updateCanvasAndBlock();

        while (utils::negativeOrientation(_complexPoints[i].getValue(), _complexPoints[i].getPrevPoint()->getValue(), _complexPoints[i].getPrevPoint()->getPrevPoint()->getValue()))
        {
            (_complexPoints[i].getPrevPoint()->getPrevPoint())->setNextPoint(&_complexPoints[i]);
            _complexPoints[i].setPrevPoint(_complexPoints[i].getPrevPoint()->getPrevPoint());
            --_numberOfProcessedPoints;
            AlgorithmBase_updateCanvasAndBlock();
        }

        while (utils::negativeOrientation(_complexPoints[i].getNextPoint()->getNextPoint()->getValue(), _complexPoints[i].getNextPoint()->getValue(), _complexPoints[i].getValue()))
        {
            (_complexPoints[i].getNextPoint()->getNextPoint())->setPrevPoint(&_complexPoints[i]);
            _complexPoints[i].setNextPoint(_complexPoints[i].getNextPoint()->getNextPoint());
            --_numberOfProcessedPoints;
            AlgorithmBase_updateCanvasAndBlock();
        }
    }

    // create convex hull vector
    const Point *currentPoint = &(_complexPoints[0]);
    _convexHull.push_back(currentPoint->getValue());
    for (unsigned int i = 0; i < _numberOfProcessedPoints - 1; ++i)
    {
        currentPoint = currentPoint->getNextPoint();
        _convexHull.push_back(currentPoint->getValue());
    }

    // end animation
    emit animationFinished();
}

//-----------------------------------------------------------------------------
// drawAlgorithm
//-----------------------------------------------------------------------------
void IncrementalInsertion::drawAlgorithm(QPainter &painter) const
{
    // set painter
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen p = painter.pen();

    // draw sweep line
    p.setWidth(1);
    p.setColor(Qt::darkGray);
    painter.setPen(p);
    painter.drawLine(_xPositionOfSweepline, 0, _xPositionOfSweepline, _pRenderer->height());

    // draw convex hull
    p.setWidth(2);
    p.setColor(Qt::darkBlue);
    painter.setPen(p);
    const Point *currentPoint = &(_complexPoints[0]);
    if (_numberOfProcessedPoints > 1)
    {
        for (unsigned int i = 0; i < _numberOfProcessedPoints+1; ++i)
        {
            painter.drawLine(currentPoint->getValue(), currentPoint->getNextPoint()->getValue());
            currentPoint = currentPoint->getNextPoint();
        }
    }

    // draw points
    p.setWidth(7);
    p.setCapStyle(Qt::RoundCap);
    p.setColor(Qt::darkRed);
    painter.setPen(p);
    for (const QPoint &pt : _points)
    {
        painter.drawPoint(pt);
    }
}

//-----------------------------------------------------------------------------
// runNaiveAlgorithm
//-----------------------------------------------------------------------------
void IncrementalInsertion::runNaiveAlgorithm()
{
    ConvexHull::runNaiveAlgorithm();
}

//-----------------------------------------------------------------------------
// _whichSide
//-----------------------------------------------------------------------------
bool IncrementalInsertion::_whichSide(const QPoint p, const QPoint p1, const QPoint p2) const
{
    return ((p.x() - p1.x())*(p2.y()-p1.y())-(p.y()-p1.y())*(p2.x()-p1.x()) < 0) ? false : true;
}

//-----------------------------------------------------------------------------
// _compare
//-----------------------------------------------------------------------------
bool IncrementalInsertion::_compare(const QPoint &p1, const QPoint &p2) const
{
    if (p1.x() < p2.x())
    {
        return true;
    }
    else if (p1.x() > p2.x())
    {
        return false;
    }
    else
    {
        if (p1.y() < p2.y())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

//-----------------------------------------------------------------------------
// getConvexHull
//-----------------------------------------------------------------------------
std::vector<QPoint> IncrementalInsertion::getConvexHull() const
{
    return _convexHull;
}

//-----------------------------------------------------------------------------
// getConvexHullTest
//-----------------------------------------------------------------------------
std::vector<QPoint> IncrementalInsertion::getConvexHullTest() const
{
    return ConvexHull::convexHullTest();
}

//-----------------------------------------------------------------------------
// _checkCollinearity
//-----------------------------------------------------------------------------
Point* IncrementalInsertion::_checkCollinearity(Point &x1, Point &x2, Point &x3) const
{
    QPoint p1 = x1.getValue();
    QPoint p2 = x2.getValue();
    QPoint p3 = x3.getValue();

    if ((p1.x() * (p2.y() - p3.y()) + p2.x() * (p3.y() - p1.y()) + p3.x() * (p1.y() - p2.y())) == 0)
    {
        return &x1;
    }
    else
    {
        return nullptr;
    }
}

