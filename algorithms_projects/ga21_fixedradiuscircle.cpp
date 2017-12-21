#include "ga021_fixedradiuscircle.h"


FixedRadiusCircle::FixedRadiusCircle(QWidget *pRenderer, int delayMs, std::string filename)
:AlgorithmBase{pRenderer, delayMs}
{
    Q_UNUSED(filename);
    Q_UNUSED(_radius);
}

void FixedRadiusCircle::runAlgorithm()
{
    emit animationFinished();
}

void FixedRadiusCircle::drawAlgorithm(QPainter &painter) const
{
    Q_UNUSED(painter);
}


void FixedRadiusCircle::runNaiveAlgorithm()
{

}
