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

    _localNearestPairs = {};
    _middleLines = {};
    _candidates = {};
    _distance = -1;
    _currentFirst = nullptr;
    _currentSecond = nullptr;
}

void NearestPoints::runAlgorithm()
{
    //sortes the points by the x axis O(nlogn)
    std::sort(_points.begin(), _points.end(), [](const QPoint &p1, const QPoint &p2) {
        return p1.x() < p2.x();
    });

    _pointsCopy = _points; //make a copy of the sorted points

    AlgorithmBase_updateCanvasAndBlock();

    QPair<QPoint, QPoint> nearestPair;
    findNearestPoints(0, _points.size(), nearestPair); //O(nlogn) - T(N) = 2T(N/2) + cN
    if(_destroyAnimation)
        return;

    AlgorithmBase_updateCanvasAndBlock();

    emit animationFinished();
}

void NearestPoints::drawAlgorithm(QPainter &painter) const
{
    int length = (int) _pointsCopy.size();
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen = painter.pen();
    pen.setCapStyle(Qt::RoundCap);

    //draws all the points (points outside the frame are painted in gray)
    for(int i = 0; i < length; i++) {
        if(i >= _leftIndex && i < _rightIndex)
            changePen(painter, pen, 5);
        else
            changePen(painter, pen, 5, Qt::gray);
        painter.drawPoint(_pointsCopy[i]);
    }

    //draw current frame
    if(_middleLines.size() != 0) {
        drawCurrentSubproblemFrame(painter, pen);
    }

    //draw local solutions
    for(const QPair<QPoint, QPoint> &pair : _localNearestPairs) {
        drawNearestPair(painter, pen, pair.first, pair.second);
    }

    //draw selected points and their segment line (if they exist)
    if(_currentFirst != nullptr && _currentSecond != nullptr) {
        drawCurrentlySelectedPoints(painter, pen);
    }

    //paint the candidates with a new color
    changePen(painter, pen, 5, Qt::green);
    for(const QPoint &point : _candidates) {
        painter.drawPoint(point);
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
    if(right - left == 2) {
        _leftIndex = left;
        _rightIndex = right;
        merge(left, left + 1, right);
        result = {_points[left], _points[left + 1]};
        _localNearestPairs.push_back({result.first, result.second});
        AlgorithmBase_updateCanvasAndBlock();
        return;
    } else if(right - left == 3) {
        _leftIndex = left;
        _rightIndex = right;
        sort3(left);

        double d1 = utils::distance(_points[left], _points[left + 1]);
        double d2 = utils::distance(_points[left], _points[left + 2]);
        double d3 = utils::distance(_points[left + 1], _points[left + 2]);

        double m = std::min(std::min(d1, d2), std::min(d2, d3));
        if(d1 == m) result = {_points[left], _points[left + 1]};
        else if(d2 == m) result = {_points[left], _points[left + 2]};
        else result = {_points[left + 1], _points[left + 2]};

        _localNearestPairs.push_back({result.first, result.second});
        AlgorithmBase_updateCanvasAndBlock();
        return;
    }

    _leftIndex = left;
    _rightIndex = right;
    _middleIndex = left + (right - left) / 2;
    _middleLines.push_back(_points[_middleIndex - 1].x() + (_points[_middleIndex].x() - _points[_middleIndex - 1].x()) / 2);
    AlgorithmBase_updateCanvasAndBlock();

    QPair<QPoint, QPoint> nearestLeft;
    QPair<QPoint, QPoint> nearestRight;

    if(_destroyAnimation)
        return;
    findNearestPoints(left, _middleIndex, nearestLeft);

    //restore middle index
    _middleIndex = left + (right - left) / 2;

    if(_destroyAnimation)
        return;
    findNearestPoints(_middleIndex, right, nearestRight);

    if(_destroyAnimation)
        return;

    //restore left, right, and middle indexes
    _leftIndex = left;
    _rightIndex = right;
    _middleIndex = left + (right - left) / 2;

    merge(_leftIndex, _middleIndex, _rightIndex);

    double d1 = utils::distance(nearestLeft.first, nearestLeft.second);
    double d2 = utils::distance(nearestRight.first, nearestRight.second);
    if(d1 <= d2) {
        _distance = d1;
        result = nearestLeft;
    } else {
        _distance = d2;
        result = nearestRight;
    }

    //filter candidates
    for(int i = left; i < right; i++)
        if(fabs(_middleLines.back() - _points[i].x()) < _distance)
            _candidates.push_back(_points[i]);

    //merge solutions
    double min = -1, tmp;
    QPair<QPoint, QPoint> tmp1;
    if(_candidates.size() >= 2) {
        for(int i = 0; i < _candidates.size() - 1; i++) {
            _currentFirst = &_candidates[i];
            for(int j = i + 1; j <= i + 7 && j < _candidates.size(); j++) {
                _currentSecond = &_candidates[j];
                AlgorithmBase_updateCanvasAndBlock();
                tmp = utils::distance(*_currentFirst, *_currentSecond);
                if(tmp < min || min == -1) {
                    min = tmp;
                    tmp1 = {*_currentFirst, *_currentSecond};
                }
                _currentSecond = nullptr;
            }
            _currentFirst = nullptr;
        }
    }

    _localNearestPairs.pop_back();
    _localNearestPairs.pop_back();
    if(min < _distance && min != -1) {
        result = tmp1;
    }
    _localNearestPairs.push_back(result);

    _candidates = {};

    AlgorithmBase_updateCanvasAndBlock();

    _distance = -1;
    _middleLines.pop_back();
}

void NearestPoints::merge(int left, int mid, int right)
{
    std::vector<QPoint> l = std::vector<QPoint>(_points.begin() + left, _points.begin() + mid);
    std::vector<QPoint> r = std::vector<QPoint>(_points.begin() + mid, _points.begin() + right);

    int i = 0;
    int j = 0;
    int k = 0;

    while(i < mid - left && j < right - mid) {
        if(l[i].y() < r[j].y()) {
            _points[left + k] = l[i];
            i++;
        } else {
            _points[left + k] = r[j];
            j++;
        }
        k++;
    }

    while(i < mid - left) {
        _points[left + k] = l[i];
        i++;
        k++;
    }

    while(j < right - mid) {
        _points[left + k] = r[j];
        j++;
        k++;
    }
}

void NearestPoints::sort3(int left)
{
    for(int i = left; i < left + 3 - 1; i++) {
        for(int j = i + 1; j < left + 3; j++) {
            if(_points[i].y() > _points[j].y()) {
                QPoint tmp = _points[i];
                _points[i] = _points[j];
                _points[j] = tmp;
            }
        }
    }
}

/*
 * Helper methods for drawing
 */

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

    if(_rightIndex - _leftIndex > 3) {
        //draws vertical line
        changePen(painter, pen, 3);
        painter.drawLine(_middleLines.back(), 0, _middleLines.back(), _pRenderer->height());
    }

    if(_distance != -1) {
        //draws two striped lines for the candidates
        changePen(painter, pen, 1, Qt::black, Qt::PenStyle::DashLine);
        painter.drawLine(_middleLines.back() - _distance, 0, _middleLines.back() - _distance, _pRenderer->height());
        painter.drawLine(_middleLines.back() + _distance, 0, _middleLines.back() + _distance, _pRenderer->height());
    }
}

void NearestPoints::drawCurrentlySelectedPoints(QPainter &painter, QPen &pen) const
{
    changePen(painter, pen, 2, Qt::yellow);
    painter.drawLine(*_currentFirst, *_currentSecond);

    changePen(painter, pen, 5, Qt::green);
    painter.drawPoint(*_currentFirst);
    painter.drawPoint(*_currentSecond);
}
