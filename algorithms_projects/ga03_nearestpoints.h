#ifndef GA03_NEARESTPOINTS_H
#define GA03_NEARESTPOINTS_H

#include <algorithmbase.h>

class NearestPoints : public AlgorithmBase
{
public:
    NearestPoints(QWidget* pRenderer, int delayMs, std::string filename = "");

    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

private:
    std::vector<QPoint> _points;
};

#endif // GA03_NEARESTPOINTS_H
