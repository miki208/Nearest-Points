#ifndef GA021_FIXEDRADIUSCIRCLE_H
#define GA021_FIXEDRADIUSCIRCLE_H

#include <QWidget>
#include <QPainter>

#include "utils.h"

#include "algorithmbase.h"

#define ENTER 0
#define LEAVE 1

class FixedRadiusCircle : public AlgorithmBase
{
public:
    FixedRadiusCircle(QWidget *pRenderer, int delayMs, std::string filename = "");

public:
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

private:
    int _radius;
    std::vector<QPoint> _points;

    QPoint _currentPoint;
    QPoint _referentPoint;

    QPoint _currentCircle;
    QPoint _currentMaxCircle;
    QPoint _globalMaxCircle;

    int _globalMaxCount;
    int _currentCount;
    int _currentMaxCount;

    bool _enterLeave;
};

#endif // GA021_FIXEDRADIUSCIRCLE_H
