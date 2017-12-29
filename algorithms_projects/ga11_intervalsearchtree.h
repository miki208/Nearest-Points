/*
Autor: Ozren Demonja
Godina: 2018
Kratak opis problema: Odredjivanje preseka duzi na pravoj koriscenjem strukture interval search tree
*/

#ifndef GA11_INTERVALSEARCHTREE_H
#define GA11_INTERVALSEARCHTREE_H

#include "algorithmbase.h"

class IntervalSearchTree : public AlgorithmBase{
public:
    IntervalSearchTree(QWidget* pRenderer, int delayMs, std::string filename = "", int inputLineCheckSize = DEFAULT_LINES_NUM, int inputSize = DEFAULT_LINES_NUM);

    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

    void insert(const QLineF &line);
    void findOverlap();

    void setLine(const QLineF &line);
    void setInputLines(std::vector<QLineF> &input);
    void setLineIntervals(std::vector<QLineF> &intervalLine);

    std::vector<QLineF> getOverlapVector();
    std::vector<QLineF> getNaiveOverlapVector();

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
    std::vector<QLineF> inputLineVector;    //input lines
    std::vector<QLineF> overlapVector;      //optimal algorithm result
    std::vector<QLineF> naiveOverlapVector; //naive algorithm result
    std::vector<QLineF> intervalLineVector; //lines for which will be checked overlap
    Node *currentSearchNode;                //only used for animation current search node

    void insert(Node *node, Node *parent, const QLineF &line);
    void deleteTree(Node *root);

    Node* overlapSearch(Node *root, const QLineF &line);
    inline void findOverlap(const QLineF &line, Node *root);
    inline bool existOverlap(const QLineF &line1, const QLineF &line2);

    bool isBlackNode(const Node *node) const;
    void doubleRotation(Node *node, Node *child, bool leftRotation);
    void rotateLeft(Node *parent);
    void rotateRight(Node *parent);
    void updateMaxNode(Node *node);
    void fixInsertTree(Node *child);
    void rotationFixColor(Node *grandparent);
    void setChild(Node *parent, Node *child, bool setLeft);
    Node* getSibling(const Node *child);

    void drawTree(QPainter &painter, int x, int y, Node *root) const;
};

#endif // GA11_INTERVALSEARCHTREE_H
