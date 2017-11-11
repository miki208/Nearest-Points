#include "ga03_nearestpoints.h"
#include "utils.h"

#include <algorithm>
#include <functional>
#include <QPainter>

NearestPoints::NearestPoints(QWidget *pRenderer, int delayMs, std::string filename):AlgorithmBase{pRenderer, delayMs}
{
    if(filename == "")
        _points = generateRandomPoints();
    else
        _points = readPointsFromFile(filename);

    completed = false;
    _middleLines = {};
    _currentFirst = nullptr;
    _currentSecond = nullptr;
}

void NearestPoints::runAlgorithm()
{
    //sort the points by the x axis
    std::sort(_points.begin(), _points.end(), [](const QPoint &p1, const QPoint &p2) {
        return p1.x() < p2.x();
    });

    _pointsCopy = _points;

    AlgorithmBase_updateCanvasAndBlock();

    QPair<QPoint, QPoint> nearestPair;
    findNearestPoints(0, _points.size(), nearestPair);
    _nearestPair = nearestPair;

    emit animationFinished();
}

void NearestPoints::drawAlgorithm(QPainter &painter) const
{
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen = painter.pen();
    pen.setCapStyle(Qt::RoundCap);

    //draws all the points
    changePen(painter, pen, 5);
    for(const QPoint &point : _points)
        painter.drawPoint(point);

    if(_middleLines.size() != 0) {
        drawCurrentSubproblemFrame(painter, pen);
    }

    if(_currentFirst != nullptr && _currentSecond != nullptr) {
        drawCurrentlySelectedPoints(painter, pen);
    }

    if(completed) {
        drawNearestPair(painter, pen, _nearestPair.first, _nearestPair.second);
    }
}

void NearestPoints::runNaiveAlgorithm()
{
    unsigned length = _points.size();
    unsigned i = 0;
    unsigned j = 1;
    double minDistance, tmpDistance;

    _nearestPair = {_points[i], _points[j]};
    minDistance = utils::distance(_nearestPair.first, _nearestPair.second);

    for(; i < length - 1; i++) {
        for(j = i + 1; j < length; j++) {
            if((tmpDistance = utils::distance(_points[i], _points[j])) < minDistance) {
                _nearestPair = {_points[i], _points[j]};
                minDistance = tmpDistance;
            }
        }
    }

    AlgorithmBase_updateCanvasAndBlock();
}

void NearestPoints::findNearestPoints(int left, int right, QPair<QPoint, QPoint> &result)
{
    _leftIndex = left;
    _rightIndex = right;
    _middleIndex = _leftIndex + (_rightIndex - _leftIndex) / 2;
    _middleLines.push_back(_points[_middleIndex - 1].x() + (_points[_middleIndex].x() - _points[_middleIndex - 1].x()) / 2);
    AlgorithmBase_updateCanvasAndBlock();
}

void NearestPoints::changePen(QPainter &painter, QPen &pen, int width, const QColor &color, Qt::PenStyle style) const
{
    pen.setWidth(width);
    pen.setColor(color);
    pen.setStyle(style);
    painter.setPen(pen);
}

void NearestPoints::drawNearestPair(QPainter &painter, QPen &pen, const QPoint &p1, const QPoint &p2) const
{
    changePen(painter, pen, 2, Qt::red);
    painter.drawLine(p1, p2);

    changePen(painter, pen, 5, Qt::blue);
    painter.drawPoint(p1);
    painter.drawPoint(p2);
}

void NearestPoints::drawCurrentSubproblemFrame(QPainter &painter, QPen &pen) const
{
    //draws left and right bound
    changePen(painter, pen, 1, Qt::gray);
    painter.fillRect(0, 0, _pointsCopy[_leftIndex].x() - 4, _pRenderer->height(), Qt::Dense4Pattern);
    painter.fillRect(_pointsCopy[_rightIndex - 1].x() + 4, 0,
            _pRenderer->width() - (_pointsCopy[_rightIndex - 1].x() + 4), _pRenderer->height(), Qt::Dense4Pattern);

    //draws vertical line
    changePen(painter, pen, 3);
    painter.drawLine(_middleLines.back(), 0, _middleLines.back(), _pRenderer->height());
}

void NearestPoints::drawCurrentlySelectedPoints(QPainter &painter, QPen &pen) const
{
    changePen(painter, pen, 2);
    painter.drawLine(*_currentFirst, *_currentSecond);

    changePen(painter, pen, 5, Qt::red);
    painter.drawPoint(*_currentFirst);
    painter.drawPoint(*_currentSecond);
}
