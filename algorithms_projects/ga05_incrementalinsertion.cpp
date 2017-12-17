#include <QPainter>

#include "ga05_incrementalinsertion.h"
#include "utils.h"


//-----------------------------------------------------------------------------
// Incremental insertion
//-----------------------------------------------------------------------------
IncrementalInsertion::IncrementalInsertion(QWidget *pRenderer, int delayMs, std::string filename, int inputSize)
    : ConvexHull(pRenderer, delayMs, filename, inputSize),
      _xPositionOfSweepline(0), _numberOfPointsInHull(0), _hasProblematicPoints(true)
{}

//-----------------------------------------------------------------------------
// runAlgorithm
//-----------------------------------------------------------------------------
void IncrementalInsertion::runAlgorithm()
{
    // sort raw points by their x-coordinates
    std::sort(_points.begin(), _points.end(), [&](const QPoint& lhs, const QPoint& rhs){ return _compare(lhs, rhs); });

    // create complex points
    for (const QPoint &p : _points)
    {
        _complexPoints.push_back(Point(p));
    }

    AlgorithmBase_updateCanvasAndBlock();

    // init
    _complexPoints[0].setNextPoint(&_complexPoints[1]);
    _complexPoints[0].setPrevPoint(&_complexPoints[1]);
    _complexPoints[1].setNextPoint(&_complexPoints[0]);
    _complexPoints[1].setPrevPoint(&_complexPoints[0]);

    // process the 1st point
    _xPositionOfSweepline = _complexPoints[0].getValue().x();
    ++_numberOfPointsInHull;
    AlgorithmBase_updateCanvasAndBlock();

    // process the 2nd point
    _xPositionOfSweepline = _complexPoints[1].getValue().x();
    ++_numberOfPointsInHull;
    AlgorithmBase_updateCanvasAndBlock();

    Point *firstProblematicPoint = nullptr;
    if (_complexPoints[0].getValue().x() == _complexPoints[1].getValue().x())
    {
        firstProblematicPoint = &_complexPoints[0];
    }
    else
    {
        _hasProblematicPoints = false;
    }

    for (int i = 2; i < _complexPoints.size(); ++i)
    {
        if (_hasProblematicPoints)
        {
            if (_complexPoints[i].getValue().x() == _complexPoints[i-1].getValue().x())
            {
                _complexPoints[i].setNextPoint(firstProblematicPoint);
                _complexPoints[i].setPrevPoint(&_complexPoints[i-1]);
                firstProblematicPoint->setPrevPoint(&_complexPoints[i]);
                _complexPoints[i-1].setNextPoint(&_complexPoints[i]);

                continue;
            }

            _hasProblematicPoints = false;
        }

        ++_numberOfPointsInHull;
        _xPositionOfSweepline = _complexPoints[i].getValue().x();

        if (_complexPoints[i].getValue().y() < _complexPoints[i-1].getValue().y())    // curPoint is above prevPoint
        {
            if (!_whichSide(_complexPoints[i].getValue(), _complexPoints[i-1].getValue(), _complexPoints[i-1].getPrevPoint()->getValue()))
            {
                _complexPoints[i].setNextPoint(_complexPoints[i-1].getNextPoint());
                _complexPoints[i].setPrevPoint(&_complexPoints[i-1]);
                _complexPoints[i-1].getNextPoint()->setPrevPoint(&_complexPoints[i]);
                _complexPoints[i-1].setNextPoint(&_complexPoints[i]);
            }
            else
            {
                _complexPoints[i].setNextPoint(&_complexPoints[i-1]);
                _complexPoints[i].setPrevPoint(_complexPoints[i-1].getPrevPoint());
                _complexPoints[i-1].getPrevPoint()->setNextPoint(&_complexPoints[i]);
                _complexPoints[i-1].setPrevPoint(&_complexPoints[i]);
            }
        }
        else                                                            // curPoint is below prevPoint
        {
            if (!_whichSide(_complexPoints[i].getValue(), _complexPoints[i-1].getValue(), _complexPoints[i-1].getPrevPoint()->getValue()))
            {
                _complexPoints[i].setNextPoint(_complexPoints[i-1].getNextPoint());
                _complexPoints[i].setPrevPoint(&_complexPoints[i-1]);
                _complexPoints[i-1].getNextPoint()->setPrevPoint(&_complexPoints[i]);
                _complexPoints[i-1].setNextPoint(&_complexPoints[i]);
            }
            else
            {
                _complexPoints[i].setNextPoint(&_complexPoints[i-1]);
                _complexPoints[i].setPrevPoint(_complexPoints[i-1].getPrevPoint());
                _complexPoints[i-1].getPrevPoint()->setNextPoint(&_complexPoints[i]);
                _complexPoints[i-1].setPrevPoint(&_complexPoints[i]);
            }
        }
        AlgorithmBase_updateCanvasAndBlock();

        while (utils::negativeOrientation(_complexPoints[i].getValue(), _complexPoints[i].getPrevPoint()->getValue(), _complexPoints[i].getPrevPoint()->getPrevPoint()->getValue()))
        {
            (_complexPoints[i].getPrevPoint()->getPrevPoint())->setNextPoint(&_complexPoints[i]);
            _complexPoints[i].setPrevPoint(_complexPoints[i].getPrevPoint()->getPrevPoint());
            AlgorithmBase_updateCanvasAndBlock();
        }
        AlgorithmBase_updateCanvasAndBlock();

        while (utils::negativeOrientation(_complexPoints[i].getNextPoint()->getNextPoint()->getValue(), _complexPoints[i].getNextPoint()->getValue(), _complexPoints[i].getValue()))
        {
            (_complexPoints[i].getNextPoint()->getNextPoint())->setPrevPoint(&_complexPoints[i]);
            _complexPoints[i].setNextPoint(_complexPoints[i].getNextPoint()->getNextPoint());
            AlgorithmBase_updateCanvasAndBlock();
        }

        AlgorithmBase_updateCanvasAndBlock();
    }

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
    p.setColor(QColor(30, 100, 255));
    painter.setPen(p);
    painter.drawLine(_xPositionOfSweepline, 0, _xPositionOfSweepline, _pRenderer->height());

    // draw convex hull
    p.setWidth(2);
    p.setColor(QColor(0, 130, 255));
    painter.setPen(p);
    const Point *currentPoint = &(_complexPoints[0]);
    if (_numberOfPointsInHull > 1)
    {
        for (int i = 0; i < _numberOfPointsInHull+1; ++i)
        {
            painter.drawLine(currentPoint->getValue(), currentPoint->getNextPoint()->getValue());
            currentPoint = currentPoint->getNextPoint();
        }
    }

    // draw points
    p.setWidth(4);
    p.setCapStyle(Qt::RoundCap);
    p.setColor(Qt::red);
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
bool IncrementalInsertion::_whichSide(QPoint x, QPoint x1, QPoint x2)
{
    return ((x.x() - x1.x())*(x2.y()-x1.y())-(x.y()-x1.y())*(x2.x()-x1.x()) < 0) ? false : true;
}

//-----------------------------------------------------------------------------
// _compare
//-----------------------------------------------------------------------------
bool IncrementalInsertion::_compare(const QPoint &p1, const QPoint &p2)
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
// getConvexHullTest
//-----------------------------------------------------------------------------
std::vector<QPoint> IncrementalInsertion::getConvexHullTest() const
{
    return ConvexHull::convexHullTest();
}
