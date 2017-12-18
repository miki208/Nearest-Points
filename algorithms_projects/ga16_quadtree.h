#ifndef GA16_QUADTREE_H
#define GA16_QUADTREE_H

#include "algorithmbase.h"

class Quadtree : public AlgorithmBase
{
public:
    Quadtree(QWidget* pRenderer, int delayMs, std::string filename = "");

    // AlgorithmBase interface
public:
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();
private:
    int sweepY;
    std::vector<QPoint> points;
};

#endif // GA16_QUADTREE_H
