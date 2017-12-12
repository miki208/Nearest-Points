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
///

enum VertexType {START, SPLIT, MERGE, END, REGULAR};

class Vertex
{
public:
    Vertex();
    Vertex(QPoint components, HalfEdge* incidentEdge);

    QPoint coordinates() const;
    void setCoordinates(const QPoint &coordinates);

    HalfEdge *incidentEdge() const;
    void setIncidentEdge(HalfEdge *incidentEdge);

    VertexType type() const;
    void setType(const VertexType &type);

private:
    QPoint _coordinates;
    HalfEdge* _incidentEdge;

    //Additional data for triangulation
    VertexType _type;
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

    Vertex *helper() const;
    void setHelper(Vertex *helper);

    std::string name() const;
    void setName(const std::string &name);

private:
    Vertex* _origin;
    HalfEdge* _twin;
    HalfEdge* _next;
    HalfEdge* _prev;
    Field* _incidentFace;

    //Additional data for triangulation
    Vertex* _helper;
    std::string _name;
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

    //TODO - test!
    void split(Vertex* v1, Vertex* v2);
    void split2(Vertex* v1, Vertex* v2, HalfEdge* e1_next, HalfEdge* e1_prev, HalfEdge* e2_next, HalfEdge* e2_prev);
    void addEdge(HalfEdge* e);
private:
    HalfEdge* findEgde(Vertex* start, Vertex* end);

    std::vector<Vertex*> _vertices;
    std::vector<HalfEdge*> _edges;
    std::vector<Field*> _fields;
};

#endif // GA04_DCEL_H
