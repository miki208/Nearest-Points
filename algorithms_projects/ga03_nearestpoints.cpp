#include "ga03_nearestpoints.h"

NearestPoints::NearestPoints(QWidget *pRenderer, int delayMs, std::string filename):AlgorithmBase{pRenderer, delayMs}
{
    if(filename == "")
        _points = generateRandomPoints();
    else
        _points = readPointsFromFile(filename);
}

void NearestPoints::runAlgorithm()
{
    emit animationFinished();
}

void NearestPoints::drawAlgorithm(QPainter &painter) const
{

}

void NearestPoints::runNaiveAlgorithm()
{

}
