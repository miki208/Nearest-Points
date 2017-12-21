#ifndef GA021_FIXEDRADIUSCIRCLE_H
#define GA021_FIXEDRADIUSCIRCLE_H

#include <QWidget>
#include <QPainter>

#include "utils.h"

#include "algorithmbase.h"

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
};

#endif // GA021_FIXEDRADIUSCIRCLE_H
