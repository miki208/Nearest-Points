#include "ga03_nearestpoints.h"
#include "utils.h"

#include <QPainter>

NearestPoints::NearestPoints(QWidget *pRenderer, int delayMs, std::string filename):AlgorithmBase{pRenderer, delayMs}
{
    if(filename == "")
        _points = generateRandomPoints();
    else
        _points = readPointsFromFile(filename);

    completed = false;
}

void NearestPoints::runAlgorithm()
{
    runNaiveAlgorithm();
    emit animationFinished();
}

void NearestPoints::drawAlgorithm(QPainter &painter) const
{
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen = painter.pen();
    pen.setCapStyle(Qt::RoundCap);

    //draw all the points
    changePen(painter, pen, 5);
    for(const QPoint &point : _points)
        painter.drawPoint(point);

    //draw the nearest points
    drawNearestPair(painter, pen, _nearestPair.first, _nearestPair.second);
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
