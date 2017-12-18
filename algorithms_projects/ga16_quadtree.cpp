#include "ga16_quadtree.h"

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



Quadtree::Quadtree(QWidget *pRenderer, int delayMs, std::string filename)
    : AlgorithmBase(pRenderer, 50), insertingFinished(false)
    // DEBUG easier testing with small delay
{
    Q_UNUSED(delayMs);
    if(filename == "")
        points = generateRandomPoints(50);
    else
        points = readPointsFromFile(filename);

    Bounds b(0, 0, _pRenderer->width(), pRenderer->height());
    root = new Node(b, 1);

    allItems.reserve(points.size());

    for (QPoint &p : points){
        allItems.push_back(new Item(p.x(), p.y(), 0, 0));
    }
}


void Quadtree::runAlgorithm()
{
    for (Item * item : allItems) {
        root->insert(item);
        AlgorithmBase_updateCanvasAndBlock();

    }
    insertingFinished = true;
    collider = allItems[0];
    collidesWith = root->retrieve(collider);
    AlgorithmBase_updateCanvasAndBlock();

    AlgorithmBase_updateCanvasAndBlock();
    emit animationFinished();
}

void Quadtree::drawAlgorithm(QPainter &painter) const
{
    if (!insertingFinished)
    root->drawSelf(painter);

    // Get all collisions of an item
    if (!insertingFinished){
        return;
    }
    qDebug() << "drugi deo";
    QPen p;
    p.setWidth(20);
    p.setColor(Qt::GlobalColor::green);
    painter.setPen(p);

    painter.fillRect(collider->x, collider->y, collider->h, collider->w,
                     Qt::GlobalColor::green);
    painter.drawPoint(collider->x, collider->y);

    qDebug() << "broj collides with = " << collidesWith.size();
    p.setColor(Qt::GlobalColor::red);

    painter.setPen(p);
    for (auto c : collidesWith){
        painter.fillRect(c->x, c->y, c->h, c->w, Qt::GlobalColor::red);
        p.setWidth(3);
        painter.setPen(p);
        painter.drawLine(collider->x, collider->y, c->x, c->y);
        p.setWidth(10);
        painter.setPen(p);
        painter.drawPoint(c->x, c->y);
    }
}

void Node::drawSelf(QPainter &painter)
{
    QPen p;
    int brightness = 50 * (depth - 1);
    p.setColor(QColor::fromRgb(brightness, brightness, brightness));
    p.setWidth(16 / depth);

    painter.setPen(p);
    painter.drawRect(bounds.x, bounds.y, bounds.w, bounds.h);

    p.setColor(QColor::fromRgb(10, 30, 170));
    painter.setPen(p);

    for (Item *c : children){
        painter.drawRect(c->x, c->y, c->w, c->h);
        painter.drawPoint(c->x, c->y);
    }

    if (!isLeafNode){
        for (int i=0; i<NUM_OF_SUBNODES; i++){
            nodes[i]->drawSelf(painter);
        }
    }
}

void Quadtree::runNaiveAlgorithm()
{

}
