#ifndef GA11_INTERVALSEARCHTREE_H
#define GA11_INTERVALSEARCHTREE_H

#include "algorithmbase.h"

class IntervalSearchTree : public AlgorithmBase{
public:
    IntervalSearchTree(QWidget* pRenderer, int delayMs, std::string filename = "", int inputSize = DEFAULT_LINES_NUM);

    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

    void insert(const QLineF &line);
    void setLine(const QLineF &line);
    void findOverlap();

    ~IntervalSearchTree(){
       deleteTree(root);
    }

protected:
    class Node {
    public:
        enum Color {RED, BLACK};

        Node(const Color color, const QLineF &line, Node *parent)
            : color(color), line(line), parent(parent),
              left(nullptr), right(nullptr)
        {
            max = line.p2().x();
        }

        Color color;
        QLineF line;
        int max;

        Node *parent;
        Node *left;
        Node *right;
    };

private:
    Node *root;
    bool static findNew;
    std::vector<QLineF> inputLineVector;    //input lines
    std::vector<QLineF> overlapVector;      //result
    std::vector<QLineF> intervalLineVector;     //lines for which will be checked overlap
    Node *currentSearchNode;                //only used for animation

    void insert(Node *node, Node *parent, const QLineF &line);
    void deleteTree(Node *root);

    bool isBlackNode(const Node *node) const;
    void doubleRotation(Node *node, Node *child, bool leftRotation);
    void rotateLeft(Node *parent);
    void rotateRight(Node *parent);
    void updateMaxNode(Node *node);
    void fixInsertTree(Node *child);
    void rotationFixColor(Node *grandparent);
    void setChild(Node *parent, Node *child, bool setLeft);
    Node* getSibling(const Node *child);

    void findOverlap(const QLineF &line, Node *root);
    bool existOverlap(const QLineF &line1, const QLineF &line2);

    void drawTree(QPainter &painter, int x, int y, Node *root) const;
};

#endif // GA11_INTERVALSEARCHTREE_H
