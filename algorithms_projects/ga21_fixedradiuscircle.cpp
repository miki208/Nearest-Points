#include "ga021_fixedradiuscircle.h"
#include <QDebug>

FixedRadiusCircle::FixedRadiusCircle(QWidget *pRenderer, int delayMs, std::string filename)
:AlgorithmBase{pRenderer, delayMs}
{
    if(filename == "") {
        _points = generateRandomPoints();
    } else {
        _points = readPointsFromFile(filename);
    }

    _radius = 70;
}

void FixedRadiusCircle::runAlgorithm()
{
    _globalMaxCount = 0;
    _currentMaxCircle = _points[0];

    std::vector<std::pair<double, std::pair<bool,QPoint>>> nearPoints;
    //                    angle             e/l  point

    for (auto current: _points) {
        for (auto point: _points) {
            double distance = utils::distance(current,point);
            if (point != current && distance <= 2*_radius) {
                _currentPoint = current;

                double B = std::acos(distance/(2*_radius));
                double A = std::atan2((current.y()-point.y()),(current.x()-point.x())) + M_PI;
                double alpha = A-B;
                double beta = A+B;
                nearPoints.push_back(std::make_pair(alpha, std::make_pair(true,point)));
                nearPoints.push_back(std::make_pair(beta, std::make_pair(false,point)));
            }
        }

        std::sort(nearPoints.begin(), nearPoints.end(),
                  [](const std::pair<double, std::pair<bool,QPoint>> & a, const std::pair<double, std::pair<bool,QPoint>> & b) -> bool
                  {
                      return a.first < b.first;
                  });

        int nearPointsCount = 1;
        _currentMaxCount = 1;

        QPoint p =  QPoint(current.x() + _radius,current.y());
        QPoint c = current;
        double angle;

        for (auto nearPoint = nearPoints.cbegin(); nearPoint!=nearPoints.cend(); ++nearPoint) {
            if ((*nearPoint).second.first) {
                _enterLeave = ENTER;
                angle = (*nearPoint).first;
                nearPointsCount++;
            } else {
                _enterLeave = LEAVE;
                angle = (*nearPoint).first;
                nearPointsCount--;
            }

            _referentPoint = (*nearPoint).second.second;
            _currentCircle = QPoint(std::cos(angle) * (p.x() - c.x()) - std::sin(angle) * (p.y() - c.y()) + c.x(),
                                    std::sin(angle) * (p.x() - c.x()) + std::cos(angle) * (p.y() - c.y()) + c.y());
            _currentCount = nearPointsCount;

            if (nearPointsCount > _currentMaxCount){
                _currentMaxCircle = _currentCircle;
                _currentMaxCount = nearPointsCount;
            }

            AlgorithmBase_updateCanvasAndBlock();
        }

        if(_currentMaxCount > _globalMaxCount){
            _globalMaxCircle = _currentMaxCircle;
            _globalMaxCount = _currentMaxCount;
        }

        nearPoints.clear();
    }

    _currentCircle.setX(-_radius);
    _currentMaxCircle.setX(-_radius);
    _currentPoint = _currentCircle;
    _referentPoint = _currentCircle;
    AlgorithmBase_updateCanvasAndBlock();

    emit animationFinished();
}

void FixedRadiusCircle::drawAlgorithm(QPainter &painter) const
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    QColor inactivePointColor = QColor(155,155,155,255);
    QColor circleColor = QColor(95,95,175,255);
    QColor maxCircleFillColor = QColor(125,125,195,100);
    QColor enterColor = QColor(0,200,50,255);
    QColor leaveColor = QColor(200,0,50,255);
    QColor currentColor = QColor(200,200,50,255);

    QPen p = painter.pen();

    p.setColor(inactivePointColor);
    p.setWidthF(1);
    painter.setPen(p);
    painter.setBrush(inactivePointColor);

    for(auto point : _points){
        painter.drawEllipse(point,2,2);
    }

    p.setColor(currentColor);
    painter.setPen(p);
    painter.setBrush(currentColor);
    painter.drawEllipse(_currentPoint, 3,3);

    if(_enterLeave == ENTER){
        painter.setPen(enterColor);
        painter.setBrush(enterColor);
    } else {
        painter.setPen(leaveColor);
        painter.setBrush(leaveColor);
    }
    painter.drawEllipse(_referentPoint,3,3);


    p.setColor(circleColor);
    p.setWidthF(1);
    painter.setPen(p);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(_currentCircle,_radius,_radius);

    painter.drawText(_currentCircle,QString::number(_currentCount));


    p.setColor(circleColor);
    p.setWidthF(2);
    painter.setPen(p);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(_currentMaxCircle,_radius,_radius);

    painter.drawText(_currentMaxCircle,QString::number(_currentMaxCount));


    p.setColor(circleColor);
    p.setWidthF(1.5);
    painter.setPen(p);
    painter.setBrush(maxCircleFillColor);
    painter.drawEllipse(_globalMaxCircle,_radius,_radius);

    painter.drawText(_globalMaxCircle,QString::number(_globalMaxCount));
}

void FixedRadiusCircle::runNaiveAlgorithm()
{

}
