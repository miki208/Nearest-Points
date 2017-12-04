#ifndef GA04_DCEL_H
#define GA04_DCEL_H


#include<QPoint>
#include <QWidget>

class Vertex;
class HalfEdge;
class Field;

///
/// \brief Teme poligona
///
class Vertex
{
public:
    Vertex();
    Vertex(QPoint components, HalfEdge* incidentEdge);

    QPoint coordinates() const;
    void setCoordinates(const QPoint &coordinates);

    HalfEdge *incidentEdge() const;
    void setIncidentEdge(HalfEdge *incidentEdge);

private:
    QPoint _coordinates;
    HalfEdge* _incidentEdge;
};

///
/// \brief Polustranica poligona
///
class HalfEdge{
public:
    HalfEdge();
    HalfEdge(Vertex* origin, HalfEdge* twin, HalfEdge* next, HalfEdge* prev, Field* incidentFace);

    HalfEdge *twin() const;
    void setTwin(HalfEdge *twin);

    HalfEdge *next() const;
    void setNext(HalfEdge *next);

    HalfEdge *prev() const;
    void setPrev(HalfEdge *prev);

    Field *incidentFace() const;
    void setIncidentFace(Field *incidentFace);

    Vertex *origin() const;
    void setOrigin(Vertex *origin);

private:
    Vertex* _origin;
    HalfEdge* _twin;
    HalfEdge* _next;
    HalfEdge* _prev;
    Field* _incidentFace;
};

///
/// \brief Polje
///
class Field{
public:
    Field();
    Field(HalfEdge* outerComponent, HalfEdge* innerComponent);

    HalfEdge *outerComponent() const;
    void setOuterComponent(HalfEdge *outerComponent);

    HalfEdge *innerComponent() const;
    void setInnerComponent(HalfEdge *innerComponent);

private:
    HalfEdge* _outerComponent;
    HalfEdge* _innerComponent;
};

///
/// \brief DCEL klasa
///
class DCEL
{
public:
    DCEL(std::string inputFileName);

    void runAlgorithm();

    std::vector<Vertex *> vertices() const;

    std::vector<HalfEdge *> edges() const;

    std::vector<Field *> fields() const;
    void setFields(const std::vector<Field *> &fields);

private:
    HalfEdge* findEgde(Vertex* start, Vertex* end);

    std::vector<Vertex*> _vertices;
    std::vector<HalfEdge*> _edges;
    std::vector<Field*> _fields;
};

#endif // GA04_DCEL_H
