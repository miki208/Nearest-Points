#include "ga16_quadtree.h"

Quadtree::Quadtree(QWidget *pRenderer, int delayMs, std::string filename)
    : AlgorithmBase(pRenderer, delayMs), sweepY(0)
{
    if(filename == "")
        points = generateRandomPoints();
    else
        points = readPointsFromFile(filename);
}


void Quadtree::runAlgorithm()
{
    while (sweepY < _pRenderer->height()) {
        sweepY += 10;
        AlgorithmBase_updateCanvasAndBlock();
    }

    emit animationFinished();
}

void Quadtree::drawAlgorithm(QPainter &painter) const
{
    QPen p = painter.pen();
    p.setColor(Qt::green);
    painter.setPen(p);
    painter.drawLine(0, sweepY, _pRenderer->width(), sweepY);

    p.setWidth(5);
    for(const QPoint& pt : points)
    {
        if(pt.y() < sweepY)
            p.setColor(Qt::red);
        else
            p.setColor(Qt::blue);


        painter.setPen(p);
        painter.drawPoint(pt);
    }
}
void Quadtree::runNaiveAlgorithm()
{

}
