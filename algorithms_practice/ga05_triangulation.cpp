#include "ga05_triangulation.h"

Triangulation::Triangulation(QWidget *pRenderer, int delayMs, std::string filename)
    :AlgorithmBase (pRenderer, delayMs), _polygon(filename)
{

}

void Triangulation::runAlgorithm()
{

}

void Triangulation::drawAlgorithm(QPainter &painter) const
{
    painter.setPen(Qt::red);
    for(int i=0; i<_polygon.edges().size(); i+=2)
    {
        painter.drawLine(_polygon.edges()[i]->origin()->coordinates(),
                         _polygon.edges()[i]->next()->origin()->coordinates());
   }
}

void Triangulation::runNaiveAlgorithm()
{

}
