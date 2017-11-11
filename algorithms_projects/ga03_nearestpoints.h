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
    void changePen(QPainter &painter, QPen &pen, int width, const QColor &color = Qt::black, Qt::PenStyle style = Qt::PenStyle::SolidLine) const;
    void drawNearestPair(QPainter &painter, QPen &pen, const QPoint &p1, const QPoint &p2) const;

    std::vector<QPoint> _points;
    QPair<QPoint, QPoint> _nearestPair; //result
    bool completed; //indicator of whether the algorithm is completed
};

#endif // GA03_NEARESTPOINTS_H
