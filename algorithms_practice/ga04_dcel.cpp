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
        x = static_cast<int>((tmpx + 1)/2.0f*1200);
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


/***********************************************************************/
/*                           HALFEDGE                                  */
/***********************************************************************/
HalfEdge::HalfEdge()
    :_origin{nullptr}, _twin{nullptr}, _next{nullptr}, _prev{nullptr}, _incidentFace{nullptr}
{}

HalfEdge::HalfEdge(Vertex *origin, HalfEdge *twin, HalfEdge *next, HalfEdge *prev, Field *incidentFace)
    :_origin{origin}, _twin{twin}, _next{next}, _prev{prev}, _incidentFace{incidentFace}
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

