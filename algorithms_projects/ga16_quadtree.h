#ifndef GA16_QUADTREE_H
#define GA16_QUADTREE_H

#include "algorithmbase.h"
#include <QDebug>
#include <algorithm>

struct Bounds
{
public:
    Bounds(int x, int y, int width, int height) : x(x), y(y),
        w(width), h(height) {}
    int x;
    int y;
    int w;
    int h;
};

class Item : public Bounds
{
public:
    Item(int x, int y, int width, int height, int number) :
        Bounds(x, y, width, height), number(number) {}
    int number;
    bool hasCollision(Item *i);
};
class Quadtree;

class Node{
public:
    enum Quadrant{UpRight, DownRight, DownLeft, UpLeft};
    static const int NUM_OF_SUBNODES = 4;
    static Quadtree *parent;
    Node *nodes[NUM_OF_SUBNODES];
    std::vector<Item*> children;
    Node(int x, int y, int width, int height, int depth);
    Node(Bounds &bounds, int depth);
    Bounds bounds;
    int depth;
    bool isLeafNode;
    std::vector<Item*> retrieve(Item &item);
    std::vector<Item*> retrieve(Item *item);
    void retrieve(Item *item, std::vector<Item*> &result);
    void insert(Item *item, bool count);
    void drawSelf(QPainter &painter);
    void split();
    std::vector<Node::Quadrant> findQuadrants(Item *item);
};

// std doesn't hash pairs, so we have to inject it in std
//namespace std
//{
//    template<> struct hash<std::pair<Item*, Item*>>
//    {
//        typedef std::pair<Item*, Item*> argument_type;
//        typedef std::size_t result_type;
//        result_type operator()(argument_type const& p) const noexcept
//        {
//            uintptr_t a = reinterpret_cast<uintptr_t>(p.first);
//            uintptr_t b = reinterpret_cast<uintptr_t>(p.second);
//            return (a & 0xFFFFFFFF) << 15 | (b & 0xFFFFFFFF);
//        }
//    };
//}

class Quadtree : public AlgorithmBase
{
public:
    Quadtree(QWidget* pRenderer, int delayMs, std::string filename = "",
             int inputSize = DEFAULT_POINTS_NUM);
    // AlgorithmBase interface
public:
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();
    static const int maxChildren = 2; // Max number of children a node can contain before it's split
    static const int maxDepth = 5; // Max number of levels a quadtree can create
    static const int lineWidth = 25;
    static void getDepthColor(int depth, int &red, int &green, int &blue);
    void setPoints(const std::vector<QPoint> &points);
    int result();
    void setSquareSize(int size);
    void addCollision(Item *a, Item *b);
private:
    std::vector<bool> hadCollision;
    int squareSize = 30;
    std::vector<QPoint> points;
    std::vector<Item*> allItems;
    // std::set<std::pair<Item*, Item*>> collisions;
    Node *root; // Root node of a quadtree
    bool insertingFinished;
    Item *collider;
    std::vector<Item*> collisionCandidates;
    void checkCollision(Item *i, std::vector<Item*> &checkWith, std::vector<Item*> &result);
    void clearResult();
};

#endif // GA16_QUADTREE_H
