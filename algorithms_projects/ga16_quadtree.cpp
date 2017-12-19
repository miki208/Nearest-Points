#include "ga16_quadtree.h"

Quadtree::Quadtree(QWidget *pRenderer, int delayMs, std::string filename, int inputSize)
    : AlgorithmBase(pRenderer, delayMs), insertingFinished(false)
{
    if(filename == "")
        points = generateRandomPoints(inputSize);
    else
        points = readPointsFromFile(filename);
    if (pRenderer){
        Bounds b(0, 0, _pRenderer->width(), pRenderer->height());
        root = new Node(b, 1);
    } else {
        Bounds b(0, 0, 1200, 500);
        root = new Node(b, 1);
    }

    if (points.size() == 0){
        return;
    }
    // add one duplicate so we get a collision
    points.push_back(points[0]);
    allItems.reserve(points.size());

    for (QPoint &p : points){
        allItems.push_back(new Item(p.x(), p.y(), 0, 0));
    }
}
void Quadtree::setPoints(const std::vector<QPoint> &points_)
{
    points = points_;
    allItems.resize(0);
    for (QPoint &p : points){
        allItems.push_back(new Item(p.x(), p.y(), 0, 0));
    }
}

void Quadtree::runAlgorithm()
{
    numOfCollisions = 0;
    // building a quadtree
    for (Item * item : allItems) {
        root->insert(item);
        AlgorithmBase_updateCanvasAndBlock();

    }
    // display checking collision of an element
    insertingFinished = true;
    if(_pRenderer){
        collider = allItems[0];
        collisionCandidates = root->retrieve(collider);
    }

    AlgorithmBase_updateCanvasAndBlock();
    emit animationFinished();

    // count collisions
    std::vector<Item*> result;
    for (Item *i : allItems){
        result.resize(0);
        collider = i;
        collisionCandidates = root->retrieve(collider);
        checkCollision(collider, collisionCandidates, result);
        numOfCollisions += result.size();
       // qDebug() << "good alg:" << result.size();
    }
   // qDebug() << numOfCollisions;
}

void Quadtree::drawAlgorithm(QPainter &painter) const
{
    root->drawSelf(painter);

    // Get all collisions of an item
    if (!insertingFinished){
        return;
    }

    // draw an item for which we are checking collision
    QPen p;
    p.setWidth(20);
    p.setColor(Qt::GlobalColor::green);
    painter.setPen(p);

    painter.fillRect(collider->x, collider->y, collider->h, collider->w,
                     Qt::GlobalColor::green);
    painter.drawPoint(collider->x, collider->y);

    // qDebug() << "broj collides with = " << collisionCandidates.size();
    p.setColor(Qt::GlobalColor::red);

    // draw all items that are maybe colliding with collider
    painter.setPen(p);
    for (auto c : collisionCandidates){
        painter.fillRect(c->x, c->y, c->h, c->w, Qt::GlobalColor::red);
        p.setWidth(3);
        painter.setPen(p);
        painter.drawLine(collider->x, collider->y, c->x, c->y);
        p.setWidth(10);
        painter.setPen(p);
        painter.drawPoint(c->x, c->y);
    }
}

void Quadtree::getDepthColor(int depth, int &red, int &green, int &blue)
{
    switch(depth){
        case 1:
           red = 0, green = 10, blue = 200;
        break;
        case 2:
           red = 0, green = 200, blue = 100;
        break;
        case 3:
           red = 220, green = 220, blue = 40;
        break;
        case 4:
            red = 244, green = 111, blue = 11;
        break;
        case 5:
            red = 200, green = 11, blue = 11;
        default:
            red = 0, green = 0, blue = 0;
        break;
    }
}
int Quadtree::result()
{
    return numOfCollisions;
}

void Node::drawSelf(QPainter &painter)
{
    QPen p;
    int r, g, b;
    if (depth == 1){
        Quadtree::getDepthColor(depth, r, g, b);
        p.setColor(QColor::fromRgb(r, g, b));
        p.setWidth(Quadtree::lineWidth);
        painter.setPen(p);

        painter.drawRect(bounds.x, bounds.y, bounds.w, bounds.h);
    }


    p.setColor(QColor::fromRgb(10, 30, 170));
    p.setWidth(5);
    painter.setPen(p);

    // draw children
    for (Item *c : children){
        painter.drawRect(c->x, c->y, c->w, c->h);
        painter.drawPoint(c->x, c->y);
    }

    if (!isLeafNode){
        // recursively draw child subnodes
        for (int i=0; i<NUM_OF_SUBNODES; i++){
            nodes[i]->drawSelf(painter);
        }
        // draw inner bounds
        p.setWidth(Quadtree::lineWidth / (depth + 1 ));
        Quadtree::getDepthColor(depth, r, g, b);
        p.setColor(QColor::fromRgb(r, g, b));
        painter.setPen(p);
        painter.drawLine(bounds.x, bounds.y + bounds.h / 2,
                         bounds.x + bounds.w,  bounds.y + bounds.h / 2);
        painter.drawLine(bounds.x + bounds.w / 2, bounds.y,
                         bounds.x + bounds.w / 2,  bounds.y + bounds.h);
    }
}

void Quadtree::runNaiveAlgorithm()
{
    numOfCollisions = 0;
    std::vector<Item*> result;
    for (Item *i : allItems){
        result.resize(0);
        collider = i;
        collisionCandidates = allItems;
        checkCollision(collider, collisionCandidates, result);
        numOfCollisions += result.size();
       // qDebug() << "naive:" << result.size();
    }
}

void Quadtree::checkCollision(Item *i, std::vector<Item*> &checkWith,
                              std::vector<Item*> &result)
{
    for (Item *c : checkWith){
        if (i->hasCollision(c) && c != i){
            result.push_back(c);
        }
    }
}



Node::Node(Bounds &bounds, int depth) : bounds(bounds), depth(depth), isLeafNode(true)
{}

Node::Node(int x, int y, int width, int height, int depth) : bounds(x, y, width, height),
    depth(depth), isLeafNode(true)
{}

std::vector<Item*> Node::retrieve(Item *item)
{
    std::vector<Item*> result; // TODO calc init len
    retrieve(item, result);
    return result;
}

std::vector<Item*> Node::retrieve(Item &item)
{
    return retrieve(&item);
}

void Node::retrieve(Item *item, std::vector<Item*> &result)
{
    if(isLeafNode){
        result.insert(result.end(), children.begin(), children.end());
        return;
    }
    nodes[findQuadrant(item)]->retrieve(item, result);
}

void Node::insert(Item *item)
{
    if (isLeafNode){
        if (depth < Quadtree::maxDepth && children.size() >= Quadtree::maxChildren){
            split();
            for (auto c : children){
                insert(c);
            }
            children.resize(0);
            insert(item);
        } else {
            children.push_back(item);
        }
    } else {
        Quadrant q = findQuadrant(item);
        nodes[q]->insert(item);
    }
}

void Node::split()
{
    if (!isLeafNode){ // Debug
        qDebug() << "ne bi trebalo da si ovde";
        return;
    }
    isLeafNode = false;
    int width =  bounds.w / 2;
    int height = bounds.h / 2;
    // these two are needed in case bounds w/h are odd
    int rightWidth = bounds.w - width;
    int upHeight = bounds.h - height;
    int xMid = bounds.x + width;
    int yMid = bounds.y + height;

    nodes[UpRight] = new Node(xMid, bounds.y, rightWidth, upHeight, depth + 1);
    nodes[UpLeft] = new Node(bounds.x, bounds.y, width, upHeight, depth + 1);
    nodes[DownRight] = new Node(xMid, yMid, rightWidth, height, depth + 1);
    nodes[DownLeft] = new Node(bounds.x, yMid, width, height, depth + 1);
}

Node::Quadrant Node::findQuadrant(Item *item)
{
    bool right = bounds.x + bounds.w / 2 < item->x;
    bool up = bounds.y + bounds.h / 2 > item->y;

    Quadrant q;
    if (right){
        if (up)
            q = Quadrant::UpRight;
        else
            q = Quadrant::DownRight;
    } else {
        if (up)
            q = Quadrant::UpLeft;
        else
            q = Quadrant::DownLeft;
    }

    return q;
}

bool Item::hasCollision(Item *i)
{
    return x <= i->x + i->w && x + w >= i->x
            && y <= i->y + i->h && y + h >= i->y;
}
