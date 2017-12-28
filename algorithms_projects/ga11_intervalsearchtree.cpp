#include "ga11_intervalsearchtree.h"

#define rx 45
#define ry 25
#define Y_MOVE 80
#define X_DOUBLE_RL 170
#define X_RLEFT 70
#define X_ROOT 200

bool IntervalSearchTree::findNew = false;

IntervalSearchTree::IntervalSearchTree(QWidget* pRenderer, int delayMs, std::string filename, int inputSize)
    : AlgorithmBase(pRenderer, delayMs), root(nullptr)
{
    if(filename == ""){
        inputLineVector = generateRandomLines(inputSize);
    }
    else{
        inputLineVector = readLinesFromFile(filename);
    }
    if(inputSize != DEFAULT_LINES_NUM){
        intervalLineVector = generateRandomLines(20000);
    }

    int k = 50;
    for(int i=0; i < intervalLineVector.size(); i++)
    {
        QPointF a = intervalLineVector[i].p1();
        QPointF b = intervalLineVector[i].p2();
        a.setY(k);
        b.setY(k);
        k += 20;
        intervalLineVector[i].setPoints(a, b);
    }

    for(int i=0; i < inputLineVector.size(); i++)
    {
        QPointF a = inputLineVector[i].p1();
        QPointF b = inputLineVector[i].p2();
        a.setY(k);
        b.setY(k);
        k += 20;
        if(inputLineVector[i].p1().x() > inputLineVector[i].p2().x()){
           inputLineVector[i].setPoints(b, a);
        }
        else {
           inputLineVector[i].setPoints(a, b);
        }
    }

}

void IntervalSearchTree::runAlgorithm(){
    //Initialize structure
    for(auto const &line: inputLineVector) {
        this->insert(line);
    }

    this->findOverlap();

    emit animationFinished();
}

void IntervalSearchTree::drawAlgorithm(QPainter &painter) const{

    painter.setFont(QFont("times",11));
    QPen pen2(Qt::red);
    pen2.setWidth(2);
    painter.setPen(pen2);
    for(auto const& line: intervalLineVector)
    {
        QString nodeStr = QStringLiteral("%1").arg(line.p1().x());
        painter.drawText(line.p1().x() - 30, line.p1().y() + 5, nodeStr);
        nodeStr = QStringLiteral("%1").arg(line.p2().x());
        painter.drawText(line.p2().x() + 5, line.p2().y() + 5, nodeStr);
        painter.drawLine(line);
    }

    QPen pen(Qt::gray);
    pen.setWidth(2);
    painter.setPen(pen);
    for(auto const &line : inputLineVector)
    {
        QString nodeStr = QStringLiteral("%1").arg(line.p1().x());
        painter.drawText(line.p1().x() - 30, line.p1().y() + 5, nodeStr);
        nodeStr = QStringLiteral("%1").arg(line.p2().x());
        painter.drawText(line.p2().x() + 5, line.p2().y() + 5, nodeStr);
        painter.drawLine(line);
    }

    painter.setPen(QPen(Qt::gray));
    drawTree(painter, painter.device()->width() / 2, 350, this->root);
}

void IntervalSearchTree::drawTree(QPainter &painter, int x, int y, Node *root) const{
    if(root == nullptr)
        return;

    if(root->parent != nullptr && root->parent->left == root){
        if(root->left != nullptr)
            painter.drawLine(QPoint(x, y), QPoint(x - X_DOUBLE_RL, y + Y_MOVE));
        if(root->right != nullptr)
            painter.drawLine(QPoint(x, y), QPoint(x + X_RLEFT, y + Y_MOVE));
    }
    else if (root->parent == nullptr){
        if(root->left != nullptr)
            painter.drawLine(QPoint(x, y), QPoint(x - X_ROOT, y + Y_MOVE));
        if(root->right != nullptr)
            painter.drawLine(QPoint(x, y), QPoint(x + X_ROOT, y + Y_MOVE));
    }
    else{
        if(root->left != nullptr)
            painter.drawLine(QPoint(x, y), QPoint(x - X_RLEFT, y + Y_MOVE));
        if(root->right != nullptr)
            painter.drawLine(QPoint(x, y), QPoint(x + X_DOUBLE_RL, y + Y_MOVE));
    }

    if(currentSearchNode == root){
        painter.setBrush(QColor(226, 226, 226));
    }
    else{
        painter.setBrush(QColor(246, 246, 246));
    }

    QRect rec(x - rx, y - ry, 2 * rx, 2 * ry);
    if(std::find(overlapVector.begin(), overlapVector.end(), root->line) != overlapVector.end()){
        QPen redPen(QPen(Qt::red));
        redPen.setWidth(3);
        painter.setPen(redPen);
    }
    painter.drawEllipse(rec);
    painter.setPen(QPen(Qt::gray));
    QString nodeStr = QStringLiteral("(%1, %2)").arg(root->line.p1().x()).arg(root->line.p2().x());
    painter.drawText(rec, Qt::AlignCenter, nodeStr);
    painter.drawText(x, y-30, QStringLiteral("%1").arg(root->max));

    if(root->parent != nullptr && root->parent->left == root){
        drawTree(painter, x - X_DOUBLE_RL, y + Y_MOVE, root->left);
        drawTree(painter, x + X_RLEFT, y + Y_MOVE, root->right);
    }
    else if (root->parent == nullptr){
        drawTree(painter, x - X_ROOT, y + Y_MOVE, root->left);
        drawTree(painter, x + X_ROOT, y + Y_MOVE, root->right);
    }
    else {
        drawTree(painter, x - X_RLEFT, y + Y_MOVE, root->left);
        drawTree(painter, x + X_DOUBLE_RL, y + Y_MOVE, root->right);
    }
}

void IntervalSearchTree::runNaiveAlgorithm(){
    for(auto const& interval: intervalLineVector) {
        for(auto const& line2: inputLineVector) {
            if (interval.p1().x() <= line2.p2().x() && interval.p2().x() >= line2.p1().x())
                overlapVector.push_back(line2);
        }
    }
}

bool IntervalSearchTree::isBlackNode(const Node *node) const{
    return node == nullptr || node->color == Node::BLACK;
}

void IntervalSearchTree::deleteTree(Node *root){
    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

void IntervalSearchTree::insert(const QLineF &line) {
    if (root == nullptr) {
        root = new Node(Node::BLACK, line, nullptr);
    }
    else {
        insert(root, nullptr, line);
    }

    AlgorithmBase_updateCanvasAndBlock();
}

void IntervalSearchTree::insert(Node *node, Node *parent, const QLineF &line){
    if(node == nullptr){
        Node *newNode = new Node(Node::RED, line, parent);
        if (line.p1().x() < parent->line.p1().x())
            parent->left = newNode;
        else
            parent->right = newNode;

        fixInsertTree(newNode);
    }
    else{
        if (line.p1().x() < node->line.p1().x()){
            node->max = node->max > line.p2().x() ? node->max : line.p2().x();
            insert(node->left, node, line);
        }
        else{
            node->max = node->max > line.p2().x() ? node->max : line.p2().x();
            insert(node->right, node, line);
        }
    }

}

IntervalSearchTree::Node* IntervalSearchTree::getSibling(const Node *child){
    Node *parent = child->parent;

    if (parent == nullptr){
        return nullptr;
    }
    else if (parent->right == child){
        return parent->left;
    }
    else {
        return parent->right;
    }
}

void IntervalSearchTree::setChild(Node *parent, Node *child, bool setLeft){
    if(child != nullptr)
        child->parent = parent;

    if(setLeft){
        parent->left = child;
    }
    else {
        parent->right = child;
    }
}

void IntervalSearchTree::fixInsertTree(Node *child) {
    if (isBlackNode(child->parent) || isBlackNode(child)) {
        return;
    }

    Node *parent = child->parent;
    Node *uncle = getSibling(parent);
    Node *grandparent = parent->parent;
    if (!isBlackNode(uncle)) {
        parent->color = Node::BLACK;
        uncle->color = Node::BLACK;

        if (grandparent->parent != nullptr) {
            grandparent->color = Node::RED;
            fixInsertTree(grandparent);
        }
    }
    else if (grandparent->left == parent) {
        if (parent->right == child) {
            doubleRotation(grandparent, child, true);
        }
        else{
            rotateRight(grandparent);
            updateMaxNode(grandparent);
            updateMaxNode(parent);
        }
        rotationFixColor(grandparent);
    }
    else {
        if (parent->left == child) {
            doubleRotation(grandparent, child, false);
        }
        else{
            rotateLeft(grandparent);
            updateMaxNode(grandparent);
            updateMaxNode(parent);
        }
        rotationFixColor(grandparent);
    }
}

void IntervalSearchTree::doubleRotation(Node *grandparent, Node *child, bool leftRotation){
        Node *parent;
        if(leftRotation){
            parent = grandparent->left;
            rotateLeft(parent);
            rotateRight(grandparent);
        }
        else {
            parent = grandparent->right;
            rotateRight(parent);
            rotateLeft(grandparent);
        }
        updateMaxNode(grandparent);
        updateMaxNode(parent);
        updateMaxNode(child);
}

void IntervalSearchTree::updateMaxNode(Node *node){
    int leftMax = node->left == nullptr ? -1 : node->left->max;
    int rightMax = node->right == nullptr ? -1 : node->right->max;
    int currentMax = node->line.p2().x();
    node->max = std::max(std::max(currentMax, leftMax), rightMax);
}

void IntervalSearchTree::rotationFixColor(Node *grandparent){
    grandparent->color = Node::RED;
    getSibling(grandparent)->color = Node::RED;
    grandparent->parent->color = Node::BLACK;
}

void IntervalSearchTree::rotateLeft(Node *parent){
    Node *child = parent->right;
    Node *grandparent = parent->parent;

    if (grandparent != nullptr){
        if (parent == grandparent->left)
            setChild(grandparent, child, true);
        else
            setChild(grandparent, child, false);
    }
    else{
        root = child;
        if (child != nullptr)
            child->parent = nullptr;
    }

    setChild(parent, child->left, false);
    setChild(child, parent, true);

    AlgorithmBase_updateCanvasAndBlock();
}

void IntervalSearchTree::rotateRight(Node *parent){
    Node *child = parent->left;
    Node *grandparent = parent->parent;

    if (grandparent != nullptr){
        if (parent == grandparent->left)
            setChild(grandparent, child, true);
        else
            setChild(grandparent, child, false);
    }
    else{
        root = child;
        if (child != nullptr)
            child->parent = nullptr;
    }

    setChild(parent, child->right, true);
    setChild(child, parent, false);

    AlgorithmBase_updateCanvasAndBlock();
}

void IntervalSearchTree::setLine(const QLineF &line){
    intervalLineVector.push_back(line);
}

void IntervalSearchTree::findOverlap(){
    for(auto const& line: intervalLineVector) {
        findOverlap(line, root);

        currentSearchNode = nullptr;
        AlgorithmBase_updateCanvasAndBlock();
    }
}

void IntervalSearchTree::findOverlap(const QLineF &line, Node *root){
    if(root == nullptr)
        return;

    currentSearchNode = root;
    AlgorithmBase_updateCanvasAndBlock();

    if(existOverlap(line, root->line)){
        overlapVector.push_back(root->line);
        IntervalSearchTree::findNew = true;
    }

    if(root->left != nullptr && root->left->max >= line.p1().x()){
        findOverlap(line, root->left);
    }
    if(IntervalSearchTree::findNew || root->left == nullptr){
        findOverlap(line, root->right);
    }
}

bool IntervalSearchTree::existOverlap(const QLineF &line1, const QLineF &line2){
    if (line1.p1().x() <= line2.p2().x() && line1.p2().x() >= line2.p1().x())
        return true;
    return false;
}
