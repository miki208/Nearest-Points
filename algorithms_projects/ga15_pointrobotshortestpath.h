#ifndef GA15_POINTROBOTSHORTESTPATH_H
#define GA15_POINTROBOTSHORTESTPATH_H

#include "algorithmbase.h"
#include "algorithms_practice/ga04_dcel.h"
#include "utils.h"

struct vertex{
    std::string _v;
    QPoint _coordinates;
    std::vector<std::pair<double, vertex*> > _edges;
    bool _visited;

    vertex(std::string v, QPoint coordinates)
        : _v(v), _coordinates(coordinates), _visited(false) {}
};

class Graph{
public:
    void addVertex(const std::string &v, const QPoint &p);
    void addEdge(const std::string &from,
                 const std::string &to, double weight);
    std::map<std::string, vertex *> vertices() const;

private:
    std::map<std::string, vertex*> _vertices;
};


class PointRobotShortestPath : public AlgorithmBase
{
public:
    PointRobotShortestPath(QWidget *pRender, int delayMs, std::string fileName);

    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

    bool segmentIntersection(QPointF p1, QPointF p2, QPointF q1, QPointF q2, QPointF *i);
    int  orientation(QPointF p1, QPointF p2, QPointF p3);
    bool onSegment(QPointF p1, QPointF p2, QPointF p3);
    int lineContainsPoint(QLineF l, QPointF p);
    bool pointInsidePolygon(QPointF p1);

private:
    DCEL _obstacles;
    Graph _visibilityGraph;
    QPoint _pStart;
    QPoint _pEnd;
};

#endif // GA15_POINTROBOTSHORTESTPATH_H
