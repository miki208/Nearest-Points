#include "algorithms_practice/ga04_dcel.h"

#include <iostream>
#include <fstream>

/***********************************************************************/
/*                               DCEL                                  */
/***********************************************************************/
DCEL::DCEL(std::string inputFileName)
    :_vertices{}, _edges{}, _fields{}
{
    std::ifstream in(inputFileName);
    std::string tmp;
    in >> tmp;
    if(tmp.compare("OFF") != 0)
    {
        std::cout << "Wrong file format: " << inputFileName << std::endl;
        exit(EXIT_FAILURE);
    }

    int vertexNum, edgeNum, fieldNum;
    in >> vertexNum >> fieldNum >> edgeNum;

    /* Pravljenje temena */
    Vertex *v = NULL;
    int x,y;
    float tmpx, tmpy, tmpz;
    for(int i=0; i<vertexNum; i++)
    {
        //z samo privremeno ignorisemo jer cemo raditi 2d.
        //Trenutno podrazumevamo da su ulazne koordinate u opsegu [-1,1]
        //in >> x >> y >> z;
        //TODO: SREDITI OVO
        in >> tmpx >> tmpy >> tmpz;
        x = static_cast<int>((tmpx + 1)/2.0f*500);
        y = static_cast<int>((tmpy + 1)/2.0f*500);
        v = new Vertex({{x, y}, NULL});
        _vertices.push_back(v);
    }

    /* Pravljenje polja i poluivica */
    for(int i=0; i<fieldNum; i++)
    {
        //Pravimo novo polje
        Field* f = new Field(nullptr, nullptr);
        _fields.push_back(f);

        //Ucitavamo broj temena trenutnog polja
        int fvNum = 0, prevIdx, currIdx;
        in >> fvNum;

        //Ucitavamo prvo teme
        in >> prevIdx;
        int firstIdx = prevIdx;

        HalfEdge* ei1, *ei2;
        std::vector<HalfEdge*> tmpHalfEdgeList;
        for(int j=1; j<fvNum; j++)
        {
            in >> currIdx;
            //Provera da li polustranice vec postoje
            HalfEdge* e = findEgde(_vertices[prevIdx], _vertices[currIdx]);
            if(nullptr != e)
            {
                prevIdx = currIdx;
                tmpHalfEdgeList.push_back(e);
                tmpHalfEdgeList.push_back(e->twin());
                continue;
            }
            ei1 = new HalfEdge(_vertices[prevIdx], nullptr, nullptr, nullptr, nullptr);
            ei2 = new HalfEdge(_vertices[currIdx], ei1, nullptr, nullptr, f);
            tmpHalfEdgeList.push_back(ei1);
            tmpHalfEdgeList.push_back(ei2);
            ei1->setTwin(ei2);

            _vertices[prevIdx]->setIncidentEdge(ei1);

            ei2->setName("e" + std::to_string(_edges.size()+1));
            _edges.push_back(ei1);
            _edges.push_back(ei2);

            prevIdx = currIdx;
        }
        //Povezujemo poslednji i prvi
        HalfEdge* e = findEgde(_vertices[prevIdx], _vertices[firstIdx]);
        if(e != NULL)
        {
            tmpHalfEdgeList.push_back(e);
            tmpHalfEdgeList.push_back(e->twin());
        }
        else
        {
            ei2->setName("e" + std::to_string(_edges.size()+1));
            ei1 = new HalfEdge(_vertices[prevIdx], nullptr, nullptr, nullptr, nullptr);
            ei2 = new HalfEdge(_vertices[firstIdx], ei1, nullptr, nullptr, f);
            ei1->setTwin(ei2);

            tmpHalfEdgeList.push_back(ei1);
            tmpHalfEdgeList.push_back(ei2);

            _vertices[prevIdx]->setIncidentEdge(ei1);
            _edges.push_back(ei1);
            _edges.push_back(ei2);
            f->setOuterComponent(ei2);
        }

        /* Postavljamo prev i next pokazivace */
        for(int k=0; k<2*fvNum; k+=2)
        {
            HalfEdge* ek1 = tmpHalfEdgeList[k];
            HalfEdge* ek2 = tmpHalfEdgeList[k+1];

            if(ek1->next() == nullptr)
            {
                ek1->setNext(tmpHalfEdgeList[(k+2)%(2*fvNum)]);
                ek2->setPrev(tmpHalfEdgeList[(k+3)%(2*fvNum)]);

                if(ek1->prev() == nullptr)
                    ek1->setPrev(tmpHalfEdgeList[(k-2+2*fvNum)%(2*fvNum)]);
                ek2->setNext(tmpHalfEdgeList[(k-1+2*fvNum)%(2*fvNum)]);
            }
            else{
                //Ivica je postojala pa treba prevezati postojece
                ek2->setNext(tmpHalfEdgeList[(k-1+2*fvNum)%(2*fvNum)]);
                ek2->setPrev(tmpHalfEdgeList[(k+3)%(2*fvNum)]);

                ek1->next()->twin()->setNext(tmpHalfEdgeList[(k+2)%(2*fvNum)]);
                tmpHalfEdgeList[(k+2)%(2*fvNum)]->setPrev(ek1->next()->twin());

                if(ek1->prev())
                {
                    ek1->prev()->twin()->setPrev(tmpHalfEdgeList[(k-2+2*fvNum)%(2*fvNum)]);
                    tmpHalfEdgeList[(k-2+2*fvNum)%(2*fvNum)]->setNext(ek1->prev()->twin());
                }
            }
        }
    }
}

HalfEdge* DCEL::findEgde(Vertex* start, Vertex* end)
{
    //Trazimo polustranicu ciji je origin start i cijem je blizancu origin end
    for(HalfEdge* e: _edges)
        if(e->origin() == start && e->twin()->origin() == end)
            return e;
    return nullptr;
}

std::vector<Vertex *> DCEL::vertices() const
{
    return _vertices;
}

std::vector<HalfEdge *> DCEL::edges() const
{
    return _edges;
}

std::vector<Field *> DCEL::fields() const
{
    return _fields;
}

void DCEL::setFields(const std::vector<Field *> &fields)
{
    _fields = fields;
}

void DCEL::split(Vertex *v1, Vertex *v2)
{
    HalfEdge* en1 = new HalfEdge(v2, nullptr,
                                 v1->incidentEdge()->prev()->twin(),
                                 v2->incidentEdge()->twin(),
                                 v2->incidentEdge()->twin()->incidentFace());
    HalfEdge* en2 = new HalfEdge(v1, en1,
                                 en1->prev()->next(),
                                 en1->next()->prev(),
                                 v1->incidentEdge()->twin()->incidentFace());

    en1->setTwin(en2);

    en1->next()->setPrev(en1);
    en1->prev()->setNext(en1);
    en2->next()->setPrev(en2);
    en2->prev()->setNext(en2);

    v2->setIncidentEdge(en1);
    v1->setIncidentEdge(en2);

    Field* f_new = new Field();
    f_new->setOuterComponent(en2);

    //Azuriranje polja za sve grane koje su sada u novom polju
    HalfEdge* tmp = en2;
    while(1)
    {
        tmp->setIncidentFace(f_new);
        tmp = tmp->next();
        if(tmp == en2)
            break;
    }

    _fields.push_back(f_new);
    _edges.push_back(en1);
    _edges.push_back(en2);
}

void DCEL::split2(Vertex *v1, Vertex *v2, HalfEdge *e1_next, HalfEdge *e1_prev, HalfEdge *e2_next, HalfEdge *e2_prev)
{
    HalfEdge* en1 = new HalfEdge(v2, nullptr,
                                 e1_next,
                                 e1_prev,
                                 e1_next->incidentFace());
    HalfEdge* en2 = new HalfEdge(v1, en1,
                                 e2_next,
                                 e2_prev,
                                 e2_prev->incidentFace());

    en1->setTwin(en2);

    e1_next->setPrev(en1);
    e1_prev->setNext(en1);
    e2_next->setPrev(en2);
    e2_prev->setNext(en2);

    v2->setIncidentEdge(en1);
    v1->setIncidentEdge(en2);

    Field* f_new = new Field();
    f_new->setOuterComponent(en2);

    //Azuriranje polja za sve grane koje su sada u novom polju
    HalfEdge* tmp = en2;
    while(1)
    {
        tmp->setIncidentFace(f_new);
        tmp = tmp->next();
        if(tmp == en2)
            break;
    }

    _fields.push_back(f_new);
    _edges.push_back(en1);
    _edges.push_back(en2);
}

void DCEL::addEdge(HalfEdge *e)
{
    _edges.push_back(e);
}
/***********************************************************************/
/*                             VERTEX                                  */
/***********************************************************************/

Vertex::Vertex()
    :_coordinates{}, _incidentEdge{nullptr}
{}

Vertex::Vertex(QPoint coordinates, HalfEdge *incidentEdge)
    :_coordinates{coordinates}, _incidentEdge{incidentEdge}
{}

QPoint Vertex::coordinates() const
{
    return _coordinates;
}

void Vertex::setCoordinates(const QPoint &coordinates)
{
    _coordinates = coordinates;
}

HalfEdge *Vertex::incidentEdge() const
{
    return _incidentEdge;
}

void Vertex::setIncidentEdge(HalfEdge *incidentEdge)
{
    _incidentEdge = incidentEdge;
}

VertexType Vertex::type() const
{
    return _type;
}

void Vertex::setType(const VertexType &type)
{
    _type = type;
}


/***********************************************************************/
/*                           HALFEDGE                                  */
/***********************************************************************/
HalfEdge::HalfEdge()
    :_origin{nullptr}, _twin{nullptr}, _next{nullptr}, _prev{nullptr},
      _incidentFace{nullptr}, _helper{nullptr}, _name("")
{}

HalfEdge::HalfEdge(Vertex *origin, HalfEdge *twin, HalfEdge *next, HalfEdge *prev, Field *incidentFace)
    :_origin{origin}, _twin{twin}, _next{next}, _prev{prev},
      _incidentFace{incidentFace}, _helper{nullptr}, _name("")
{}

HalfEdge *HalfEdge::twin() const
{
    return _twin;
}

void HalfEdge::setTwin(HalfEdge *twin)
{
    _twin = twin;
}

HalfEdge *HalfEdge::next() const
{
    return _next;
}

void HalfEdge::setNext(HalfEdge *next)
{
    _next = next;
}

HalfEdge *HalfEdge::prev() const
{
    return _prev;
}

void HalfEdge::setPrev(HalfEdge *prev)
{
    _prev = prev;
}

Field *HalfEdge::incidentFace() const
{
    return _incidentFace;
}

void HalfEdge::setIncidentFace(Field *incidentFace)
{
    _incidentFace = incidentFace;
}

Vertex *HalfEdge::origin() const
{
    return _origin;
}

void HalfEdge::setOrigin(Vertex *origin)
{
    _origin = origin;
}

Vertex *HalfEdge::helper() const
{
    return _helper;
}

void HalfEdge::setHelper(Vertex *helper)
{
    _helper = helper;
}

std::string HalfEdge::name() const
{
    return _name;
}

void HalfEdge::setName(const std::string &name)
{
    _name = name;
}

/***********************************************************************/
/*                             FIELD                                   */
/***********************************************************************/

Field::Field()
    :_outerComponent{nullptr}, _innerComponent{nullptr}
{}

Field::Field(HalfEdge *outerComponent, HalfEdge *innerComponent)
    :_outerComponent{outerComponent}, _innerComponent{innerComponent}
{}

HalfEdge *Field::outerComponent() const
{
    return _outerComponent;
}

void Field::setOuterComponent(HalfEdge *outerComponent)
{
    _outerComponent = outerComponent;
}

HalfEdge *Field::innerComponent() const
{
    return _innerComponent;
}

void Field::setInnerComponent(HalfEdge *innerComponent)
{
    _innerComponent = innerComponent;
}

