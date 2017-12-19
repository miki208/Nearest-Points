#ifndef GA16_QUADTREE_H
#define GA16_QUADTREE_H

#include "algorithmbase.h"
#include <QDebug>
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
    Item(int x, int y, int width, int height) : Bounds(x, y, width, height)
        {}
    bool hasCollision(Item *i);
};

class Node{
public:
    enum Quadrant{UpRight, DownRight, DownLeft, UpLeft};
    static const int NUM_OF_SUBNODES = 4;
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
    void insert(Item *item);
    void drawSelf(QPainter &painter);
    void split();
    Quadrant findQuadrant(Item *item);
};


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
private:
    std::vector<QPoint> points;
    std::vector<Item*> allItems;

    int numOfCollisions;
    Node *root; // Root node of a quadtree
    bool insertingFinished;
    Item *collider;
    std::vector<Item*> collisionCandidates;
    void checkCollision(Item *i, std::vector<Item*> &checkWith, std::vector<Item*> &result);
};

#endif // GA16_QUADTREE_H
