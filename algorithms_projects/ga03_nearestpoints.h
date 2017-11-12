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
    void findNearestPoints(int left, int right, QPair<QPoint, QPoint> &result);
    void changePen(QPainter &painter, QPen &pen, int width, const QColor &color = Qt::black, Qt::PenStyle style = Qt::PenStyle::SolidLine) const;
    void drawNearestPair(QPainter &painter, QPen &pen, const QPoint &p1, const QPoint &p2) const;
    void drawCurrentSubproblemFrame(QPainter &painter, QPen &pen) const;
    void drawCurrentlySelectedPoints(QPainter &painter, QPen &pen) const;
    void merge(int left, int mid, int right);
    void sort3(int left);

    std::vector<QPoint> _points;
    std::vector<QPoint> _pointsCopy; //a copy of the points (necessary to preserve order, according to the x axis)
    std::vector<int> _middleLines;
    QPair<QPoint, QPoint> _nearestPair; //result
    std::vector<QPair<QPoint, QPoint>> _localNearestPairs;
    std::vector<QPoint> _candidates;
    double _d;
    QPoint *_currentFirst, *_currentSecond; //currently selected points
    bool completed; //indicator of whether the algorithm is completed
    int _leftIndex, _rightIndex, _middleIndex;
};

#endif // GA03_NEARESTPOINTS_H
