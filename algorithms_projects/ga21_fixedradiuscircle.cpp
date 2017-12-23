#include "ga21_fixedradiuscircle.h"
#include <QDebug>

FixedRadiusCircle::FixedRadiusCircle(QWidget *pRenderer, int delayMs, int radius, std::string filename, int inputSize)
:AlgorithmBase{pRenderer, delayMs}
{
    if(filename == "") {
        _points = generateRandomPoints(inputSize);
    } else {
        _points = readPointsFromFile(filename);
    }

    _radius = radius;

    _globalMaxCount = 0;
    _currentMaxCircle = _points[0];
    // hidden
    _globalMaxCircle = QPoint(-_radius, -_radius);
}

void FixedRadiusCircle::runAlgorithm()
{
    //                    angle             e/l  point
    std::vector<std::pair<double, std::pair<bool,QPoint>>> nearPoints;

    // Picking point around wich circle rotates
    for (auto current: _points) {
        // Picking second point on circles ring
        for (auto point: _points) {
            double distance = utils::distance(current,point);
            if (point != current && distance <= 2*_radius) {
                _currentPoint = current;

                // Calculating angle of current, circles middle and point
                // (2 of them, for enter and leave event)
                double B = std::acos(distance/(2*_radius));
                double A = std::atan2((current.y()-point.y()),(current.x()-point.x())) + M_PI;
                double alpha = A-B;
                double beta = A+B;

                // Pushing point twice (when it enters the circle and when it leaves it)
                nearPoints.push_back(std::make_pair(alpha, std::make_pair(true,point)));
                nearPoints.push_back(std::make_pair(beta, std::make_pair(false,point)));
            }
        }

        // Sorting points in range by angle
        std::sort(nearPoints.begin(), nearPoints.end(),
                  [](const std::pair<double, std::pair<bool,QPoint>> & a, const std::pair<double, std::pair<bool,QPoint>> & b) -> bool
                  {
                      return a.first < b.first;
                  });

        // Circle will have current inside
        int nearPointsCount = 1;
        _currentMaxCount = 1;

        // p is center point of circle
        // p has same y coordinate as current
        // and current laying on circles ring (left)
        QPoint p =  QPoint(current.x() + _radius,current.y());
        QPoint c = current;
        double angle;

        // Points are sorted by angle.
        // That is why we can increase and decrease number of near points by one
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

            // Calculating event points coordinates for drawing
            _referentPoint = (*nearPoint).second.second;
            _currentCircle = QPoint(std::cos(angle) * (p.x() - c.x()) - std::sin(angle) * (p.y() - c.y()) + c.x(),
                                    std::sin(angle) * (p.x() - c.x()) + std::cos(angle) * (p.y() - c.y()) + c.y());
            _currentCount = nearPointsCount;

            // Updating maximum circle (and count) related to current
            if (nearPointsCount > _currentMaxCount){
                _currentMaxCircle = _currentCircle;
                _currentMaxCount = nearPointsCount;
            }

            AlgorithmBase_updateCanvasAndBlock();
        }

        // Updating global maximum circle (and count)
        if (_currentMaxCount > _globalMaxCount){
            _globalMaxCircle = _currentMaxCircle;
            _globalMaxCount = _currentMaxCount;
        }

        nearPoints.clear();
    }

    // At the end of animation currentMax and currentCircle are hidden,
    // same as current and referent point
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

    for (auto point : _points) {
        painter.drawEllipse(point,2,2);
    }

    p.setColor(currentColor);
    painter.setPen(p);
    painter.setBrush(currentColor);
    painter.drawEllipse(_currentPoint, 3,3);

    if (_enterLeave == ENTER) {
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
    int globalMax = 1;
    for (auto first: _points) {
        for (auto second: _points) {
            if (first == second || utils::distance(first,second) > 2*_radius)
                continue;

            int x1 = first.x();
            int y1 = first.y();
            int x2 = second.x();
            int y2 = second.y();

            int r = _radius;

            double q = sqrt((x2-x1)^2 + (y2-y1)^2);

            double y3 = (y1+y2)/2;
            double x3 = (x1+x2)/2;

            double x = x3 + sqrt(std::pow(r^2,2)-std::pow(q/2,2))*(y1-y2)/q;
            double y = y3 + sqrt(std::pow(r,2)-std::pow(q/2,2))*(x2-x1)/q;

            QPoint circle1 = QPoint(x,y);

            x = x3 - sqrt(std::pow(r,2)-std::pow(q/2,2))*(y1-y2)/q;
            y = y3 - sqrt(std::pow(r,2)-std::pow(q/2,2))*(x2-x1)/q;
            QPoint circle2 = QPoint(x,y);

            int localMax1 = 2;
            int localMax2 = 2;

            for (auto point: _points) {
                if (point == first || point == second)
                    continue;
                if (utils::distance(point,circle1) <= 2*_radius)
                    localMax1++;
                if (utils::distance(point,circle2) <= 2*_radius)
                    localMax2++;
            }

            if (localMax1 > globalMax)
                globalMax = localMax1;
            if (localMax2 > globalMax)
                globalMax = localMax2;
        }
    }
}
