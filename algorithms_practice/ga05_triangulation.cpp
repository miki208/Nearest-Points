#include <iostream>

#include "ga05_triangulation.h"

#include "utils.h"

Triangulation::Triangulation(QWidget *pRenderer, int delayMs, std::string filename)
    :AlgorithmBase (pRenderer, delayMs), _polygon(filename), _eventQueue(), _statusQueue(EdgeCompare(&_sweepLinePos)), _sweepLinePos(0)
{
    for(Vertex* v : _polygon.vertices())
    {
        _eventQueue.insert(v);
    }

    std::vector<HalfEdge*> edges = _polygon.edges();
    for(int i=1; i<edges.size(); i+=2)
    {
        //Proveravamo odnos temena i njegovih suseda
        Vertex* v1 = edges[i]->origin();
        Vertex* v2 = edges[i]->twin()->origin();
        Vertex* v3 = edges[i]->next()->twin()->origin();


        if(v1->coordinates().y() > v2->coordinates().y() &&
                v3->coordinates().y() > v2->coordinates().y())
        {
            if(!utils::negativeOrientation(v1->coordinates(), v2->coordinates(), v3->coordinates()))
                v2->setType(VertexType::SPLIT);
            else
                v2->setType(VertexType::START);
        }
        else if(v1->coordinates().y() < v2->coordinates().y() &&
                 v3->coordinates().y() < v2->coordinates().y())
        {
            if(!utils::negativeOrientation(v1->coordinates(), v2->coordinates(), v3->coordinates()))
                v2->setType(VertexType::MERGE);
            else
                v2->setType(VertexType::END);
        }
        else
            v2->setType(VertexType::REGULAR);
    }
}

void Triangulation::runAlgorithm()
{
    while(!_eventQueue.empty())
    {
        auto curr = _eventQueue.begin();
        Vertex* c = *curr;
        _sweepLinePos = c->coordinates().y();
        AlgorithmBase_updateCanvasAndBlock();

        HalfEdge* ei = c->incidentEdge()->twin()->next();
        HalfEdge* ei_prev = c->incidentEdge()->twin();

        auto ejit = _statusQueue.begin();
        HalfEdge* ej = *ejit;

        switch (c->type())
        {
            case START:
                ei->setHelper(c);
                _statusQueue.insert(ei);
                break;
            case END:
                if(ei_prev->helper() != nullptr && ei_prev->helper()->type() == VertexType::MERGE)
                {
                    _polygon.split(c, ei_prev->helper());
                }
                _statusQueue.erase(ei_prev);
                break;
            case SPLIT:
                _polygon.split(c, ej->helper());
                ej->setHelper(c);
                ei->setHelper(c);
                _statusQueue.insert(ei);
                break;
            case MERGE:
                if(ei_prev->helper() != nullptr && ei_prev->helper()->type() == MERGE)
                {
                    _polygon.split(c, ei_prev->helper());
                }
                _statusQueue.erase(ei_prev);
                if(ej->helper() && ej->helper()->type() == MERGE)
                {
                    _polygon.split(c, ej->helper());
                }
                ej->setHelper(c);

                break;
            case REGULAR:
                if(c->incidentEdge()->next()->origin()->coordinates().y() > c->coordinates().y())
                {
                     if(ei_prev->helper()->type() == MERGE)
                     {
                         _polygon.split(c, ei_prev->helper());
                     }
                     _statusQueue.erase(ei_prev);
                     ei->setHelper(c);
                     _statusQueue.insert(ei);
                }
                else
                {
                    if(ej->helper()->type() == MERGE)
                    {
                        _polygon.split(c, ej->helper());
                    }
                    ej->setHelper(c);
                }
                break;
        }
        AlgorithmBase_updateCanvasAndBlock();
        _eventQueue.erase(curr);
    }

    AlgorithmBase_updateCanvasAndBlock();
    _sweepLinePos = 500;
    AlgorithmBase_updateCanvasAndBlock();
}

void Triangulation::drawAlgorithm(QPainter &painter) const
{

    //Draw vertical and horizontal lines
    painter.setPen(Qt::gray);
    for(int i=0; i<501; i+=50)
    {
        painter.drawLine(i, 0, i, 500);
        painter.drawLine(0, i, 500, i);
    }

    //Draw sweep line
    painter.setPen(Qt::green);
    painter.drawLine(0, _sweepLinePos, 500, _sweepLinePos);

    painter.setPen(Qt::red);
    for(int i=0; i<_polygon.edges().size(); i+=2)
    {
        painter.drawLine(_polygon.edges()[i]->origin()->coordinates(),
                         _polygon.edges()[i]->twin()->origin()->coordinates());

//        QPointF loc = (_polygon.edges()[i]->origin()->coordinates() +
//                    _polygon.edges()[i]->twin()->origin()->coordinates())/2;
//        painter.drawText(loc, QString::fromStdString(_polygon.edges()[i+1]->name()));
    }

    for(const Vertex* v : _polygon.vertices())
    {
        if(v->type() == START)
        {
            painter.setBrush(Qt::red);
            painter.drawText(v->coordinates(), "Start");
        }
        else if(v->type() == SPLIT)
        {
            painter.setBrush(Qt::green);
            painter.drawText(v->coordinates(), "Split");
        }
        else if(v->type() == MERGE)
        {
            painter.setBrush(Qt::blue);
            painter.drawText(v->coordinates(), "Merge");
        }
        else if(v->type() == END)
        {
            painter.setBrush(Qt::yellow);
            painter.drawText(v->coordinates(), "End");
        }
        else
        {
            painter.setBrush(Qt::gray);
            painter.drawText(v->coordinates(), "Regular");
        }

        painter.drawEllipse(v->coordinates(), 5, 5);
    }
}

void Triangulation::runNaiveAlgorithm()
{

}

double Triangulation::sweepLinePos() const
{
    return _sweepLinePos;
}

EdgeCompare::EdgeCompare(double *ypos)
{
    _ypos = ypos;
}
