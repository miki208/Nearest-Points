#include "ga15_pointrobotshortestpath.h"
#include <iostream>


void Graph::addVertex(const std::string &name, const QPoint &p)
{
    std::map<std::string, vertex*>::iterator i = _vertices.find(name);
    if(i == _vertices.end())
    {
        vertex *v = new vertex(name, p);
        _vertices[name] = v;
    }
    else
        std::cout << "Vertex " << name << " already exists." << std::endl;
}

void Graph::addEdge(const std::string &from, const std::string &to, double weight)
{
    vertex *f = (_vertices.find(from)->second);
    vertex *t = (_vertices.find(to)->second);
    std::pair<double, vertex*> edge = std::make_pair(weight, t);
    f->_edges.push_back(edge);
}

std::map<std::string, vertex *> Graph::vertices() const
{
    return _vertices;
}




PointRobotShortestPath::PointRobotShortestPath(QWidget *pRender, int delayMs, std::string fileName)
    : AlgorithmBase(pRender, delayMs), _obstacles(fileName)
{
    _pStart = QPoint(90, 330);
    _pEnd = QPoint(500, 200);

    std::string start = "v_" + std::to_string(_pStart.x()) + "," + std::to_string(_pStart.y());
    _visibilityGraph.addVertex(start, _pStart);

    std::vector<HalfEdge*> edges = _obstacles.edges();
    for(unsigned i=0; i<edges.size(); i+=2)
    {
        QPoint p1 = edges[i]->origin()->coordinates();
        QPoint p2 = edges[i]->twin()->origin()->coordinates();

        std::string v = "v_" + std::to_string(p1.x()) + "," + std::to_string(p1.y());
        std::string w = "v_" + std::to_string(p2.x()) + "," + std::to_string(p2.y());

        _visibilityGraph.addVertex(v, p1);
        _visibilityGraph.addVertex(w, p2);
    }

    std::string end = "v_" + std::to_string(_pEnd.x()) + "," + std::to_string(_pEnd.y());
    _visibilityGraph.addVertex(end, _pEnd);
}

void PointRobotShortestPath::runAlgorithm()
{

/*********************************************************************
 *                             VISIBITY GRAPH                        *
 *********************************************************************/

    std::map<std::string, vertex*> vertices = _visibilityGraph.vertices();
    std::map<std::string, vertex*>::iterator it, jt;
    for(it = vertices.begin(); it!=vertices.end(); it++)
        for(jt = vertices.begin(); jt!=vertices.end(); jt++)
        {
            if(it->first != jt->first)
            {
                QPoint p1 = it->second->_coordinates;
                QPoint p2 = jt->second->_coordinates;

                bool add = true;

                std::vector<HalfEdge*> edges = _obstacles.edges();
                for(int i=0; i<edges.size(); i+=2)
                {
                    QPointF sin;
                    HalfEdge *e = edges[i];

                    QPoint q1 = e->origin()->coordinates();
                    QPoint q2 = e->twin()->origin()->coordinates();

                    bool intersect = segmentIntersection(p1, p2, q1, q2, &sin);
                    if((p1 == q1 && p2 == q2) || (p1 == q2 && p2 == q1))
                    {
                        _visibilityGraph.addEdge(it->first, jt->first, utils::distance(p1, p2));
                        AlgorithmBase_updateCanvasAndBlock();
                        break;
                    }
                    else if(intersect && sin != p1 && sin != p2)
                    {
                        add = false;
                        break;
                    }

                }

                QPointF midpoint((p1.x()+p2.x())/2, (p1.y()+p2.y())/2);
                if(add && !pointInsidePolygon(midpoint))
                {
                    _visibilityGraph.addEdge(it->first, jt->first, utils::distance(p1, p2));
                    AlgorithmBase_updateCanvasAndBlock();
                }
            }
        }

/*********************************************************************
 *                             DIJKSTRA                              *
 *********************************************************************/


}

void PointRobotShortestPath::drawAlgorithm(QPainter &painter) const
{
    QPen pen = painter.pen();
    pen.setColor(Qt::red);
    pen.setWidth(5);

    painter.setPen(pen);

    painter.drawPoint(_pStart);
    painter.drawPoint(_pEnd);

    for(int i=0; i<_obstacles.edges().size(); i+=2)
    {
        painter.drawLine(_obstacles.edges()[i]->origin()->coordinates(),
                         _obstacles.edges()[i]->next()->origin()->coordinates());
    }

    pen.setWidth(1.5);
    pen.setColor(Qt::green);
    painter.setPen(pen);

    //painter.drawLine(QPointF(50,50), QPointF(200,200));

    std::map<std::string, vertex*> verts = _visibilityGraph.vertices();
    std::map<std::string, vertex*>::iterator it;
    for(it=verts.begin(); it!=verts.end(); it++)
    {
        std::vector<std::pair<double, vertex*> > edges = it->second->_edges;
        std::vector<std::pair<double, vertex*> >::iterator jt;
        for(jt=edges.begin(); jt!=edges.end(); jt++)
        {
            //std::cout << jt->second->_coordinates.x() << " " << jt->second->_coordinates.y() << std::endl;
            painter.drawLine(it->second->_coordinates, jt->second->_coordinates);
        }

//        std::cout << it->first << " ";
    }
//    std::cout << std::endl << std::endl;
}

void PointRobotShortestPath::runNaiveAlgorithm()
{

}



bool PointRobotShortestPath::segmentIntersection(QPointF p1, QPointF p2, QPointF q1, QPointF q2, QPointF *i)
{
    int b1 = orientation(p1,p2,q1);
    int b2 = orientation(p1,p2,q2);
    int b3 = orientation(q1,q2,p1);
    int b4 = orientation(q1,q2,p2);

    utils::lineIntersection(QLineF(p1,p2), QLineF(q1,q2), i);

    if(b1 != b2 && b3 != b4) return true;

    if(b1 == 0 && onSegment(p1,q1,p2)) return true;
    if(b2 == 0 && onSegment(p1,q2,p2)) return true;
    if(b3 == 0 && onSegment(q1,p1,q2)) return true;
    if(b4 == 0 && onSegment(q1,p2,q2)) return true;

    return false;
}

int PointRobotShortestPath::orientation(QPointF p1, QPointF p2, QPointF p3)
{
    int o = (p2.y() - p1.y())*(p3.x() - p2.x()) -
            (p2.x() - p1.x())*(p3.y() - p2.y());

    if(o < 0)
        return -1;
    else if(o > 0)
        return 1;
    else
        return 0;
}

bool PointRobotShortestPath::onSegment(QPointF p1, QPointF q, QPointF p2)
{
    return  q.x() >= std::min(p1.x(), p2.x()) && q.x() <= std::max(p1.x(), p2.x()) &&
            q.y() >= std::min(p1.y(), p2.y()) && q.y() <= std::max(p1.y(), p2.y());
}

int PointRobotShortestPath::lineContainsPoint(QLineF l, QPointF p)
{
    double d =  (p.x() - l.p1().x())*(l.p2().y() - l.p1().y()) -
                (p.y() - l.p1().y())*(l.p2().x() - l.p1().x());

    if(d < 0)
        return -1;
    else if(d > 0)
        return 1;
    else
        return 0;
}

bool PointRobotShortestPath::pointInsidePolygon(QPointF p1)
{
    unsigned intersecs = 0;
    std::vector<HalfEdge*> edges = _obstacles.edges();
    for(unsigned i=0; i<edges.size(); i+=2)
    {
        HalfEdge *e = edges[i];
        QPointF q1 = e->origin()->coordinates();
        QPointF q2 = e->twin()->origin()->coordinates();
        QPointF p2 = QPointF(100000, p1.y());
        QPointF in;

        if(segmentIntersection(p1, p2, q1, q2, &in))
        {
            if(in == q1)
            {
                QPoint q3 = e->prev()->origin()->coordinates();
                if(lineContainsPoint(QLineF(p1, q1), q2) != lineContainsPoint(QLineF(p1, q1), q3))
                    intersecs++;
            }
            //MOZDA IMA BAG OVDE
            else if(in == q2)
            {
                QPoint q3 = e->twin()->prev()->origin()->coordinates();
                if(lineContainsPoint(QLineF(p1, q2), q1) != lineContainsPoint(QLineF(p1, q2), q3))
                    intersecs++;
            }
            else
                intersecs++;
        }
    }

    return intersecs%2 != 0;
}


/*
if(p1 == q1 && p2 == q2 || p1 == q2 && p2 == q1)
{

    b = true;
    break;
}
*/

