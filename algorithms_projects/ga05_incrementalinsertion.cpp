#include <QPainter>

#include "ga05_incrementalinsertion.h"
#include "../algorithms_practice/ga02_grahamscan.h"
#include "utils.h"


//-----------------------------------------------------------------------------
// Incremental insertion
//-----------------------------------------------------------------------------
IncrementalInsertion::IncrementalInsertion(QWidget *pRenderer, int delayMs, std::string filename, int inputSize)
    : AlgorithmBase {pRenderer, delayMs}, _xPositionOfSweepline(0), _numberOfPointsInHull(0), _hasProblematicPoints(true)
{
    if (filename == "")
        _rawPoints = generateRandomPoints(inputSize);
    else
        _rawPoints = readPointsFromFile(filename);
}

#include <QDebug>
//-----------------------------------------------------------------------------
// runAlgorithm
//-----------------------------------------------------------------------------
void IncrementalInsertion::runAlgorithm()
{
    // sort raw points by their x-coordinates
    std::sort(_rawPoints.begin(), _rawPoints.end(), [&](const QPoint& lhs, const QPoint& rhs){ return _compare(lhs, rhs); });

    // create complex points
    for (const QPoint &p : _rawPoints)
    {
        _points.push_back(Point(p));
    }

    AlgorithmBase_updateCanvasAndBlock();

    // init
    _points[0].setNextPoint(&_points[1]);
    _points[0].setPrevPoint(&_points[1]);
    _points[1].setNextPoint(&_points[0]);
    _points[1].setPrevPoint(&_points[0]);

    // process the 1st point
    _xPositionOfSweepline = _points[0].getValue().x();
    ++_numberOfPointsInHull;
    AlgorithmBase_updateCanvasAndBlock();

    // process the 2nd point
    _xPositionOfSweepline = _points[1].getValue().x();
    ++_numberOfPointsInHull;
    AlgorithmBase_updateCanvasAndBlock();

    Point *firstProblematicPoint = nullptr;
    if (_points[0].getValue().x() == _points[1].getValue().x())
    {
        firstProblematicPoint = &_points[0];
    }
    else
    {
        _hasProblematicPoints = false;
    }

    for (int i = 2; i < _points.size(); ++i)
    {
        if (_hasProblematicPoints)
        {
            if (_points[i].getValue().x() == _points[i-1].getValue().x())
            {
                _points[i].setNextPoint(firstProblematicPoint);
                _points[i].setPrevPoint(&_points[i-1]);
                firstProblematicPoint->setPrevPoint(&_points[i]);
                _points[i-1].setNextPoint(&_points[i]);

                continue;
            }

            _hasProblematicPoints = false;
        }

        ++_numberOfPointsInHull;
        _xPositionOfSweepline = _points[i].getValue().x();

        if (_points[i].getValue().y() < _points[i-1].getValue().y())    // curPoint is above prevPoint
        {
            if (!_whichSide(_points[i].getValue(), _points[i-1].getValue(), _points[i-1].getPrevPoint()->getValue()))
            {
                _points[i].setNextPoint(_points[i-1].getNextPoint());
                _points[i].setPrevPoint(&_points[i-1]);
                _points[i-1].getNextPoint()->setPrevPoint(&_points[i]);
                _points[i-1].setNextPoint(&_points[i]);
            }
            else
            {
                _points[i].setNextPoint(&_points[i-1]);
                _points[i].setPrevPoint(_points[i-1].getPrevPoint());
                _points[i-1].getPrevPoint()->setNextPoint(&_points[i]);
                _points[i-1].setPrevPoint(&_points[i]);
            }
        }
        else                                                            // curPoint is below prevPoint
        {
            if (!_whichSide(_points[i].getValue(), _points[i-1].getValue(), _points[i-1].getPrevPoint()->getValue()))
            {
                _points[i].setNextPoint(_points[i-1].getNextPoint());
                _points[i].setPrevPoint(&_points[i-1]);
                _points[i-1].getNextPoint()->setPrevPoint(&_points[i]);
                _points[i-1].setNextPoint(&_points[i]);
            }
            else
            {
                _points[i].setNextPoint(&_points[i-1]);
                _points[i].setPrevPoint(_points[i-1].getPrevPoint());
                _points[i-1].getPrevPoint()->setNextPoint(&_points[i]);
                _points[i-1].setPrevPoint(&_points[i]);
            }
        }
        AlgorithmBase_updateCanvasAndBlock();

        while (utils::negativeOrientation(_points[i].getValue(), _points[i].getPrevPoint()->getValue(), _points[i].getPrevPoint()->getPrevPoint()->getValue()))
        {
            (_points[i].getPrevPoint()->getPrevPoint())->setNextPoint(&_points[i]);
            _points[i].setPrevPoint(_points[i].getPrevPoint()->getPrevPoint());
            AlgorithmBase_updateCanvasAndBlock();
        }
        AlgorithmBase_updateCanvasAndBlock();

        while (utils::negativeOrientation(_points[i].getNextPoint()->getNextPoint()->getValue(), _points[i].getNextPoint()->getValue(), _points[i].getValue()))
        {
            (_points[i].getNextPoint()->getNextPoint())->setPrevPoint(&_points[i]);
            _points[i].setNextPoint(_points[i].getNextPoint()->getNextPoint());
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
    const Point *currentPoint = &(_points[0]);
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
    for (const QPoint &pt : _rawPoints)
    {
        painter.drawPoint(pt);
    }
}

//-----------------------------------------------------------------------------
// runNaiveAlgorithm
//-----------------------------------------------------------------------------
void IncrementalInsertion::runNaiveAlgorithm()
{

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
