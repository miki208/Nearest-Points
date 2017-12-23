#include "ga18_smallestenclosingdisk.h"

#include <QPainter>
#include <QDebug>


ga18_smallestEnclosingDisk::ga18_smallestEnclosingDisk(
        QWidget *pRenderer, int delayMs, std::string filename )
        :AlgorithmBase (pRenderer, delayMs), n(0)
{
    std::vector<QPoint> pointInt;
    if(filename == "")
        pointInt = generateRandomPoints();
    else
        pointInt = readPointsFromFile(filename);
    for ( auto p : pointInt){
        points.push_back(QPointF(p));
    }
}


void ga18_smallestEnclosingDisk::runAlgorithm()
{
    while ( n < points.size() ){
        AlgorithmBase_updateCanvasAndBlock();
        n++;
    }
    emit animationFinished();
}


void ga18_smallestEnclosingDisk::drawAlgorithm(QPainter &painter) const
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QPen p = painter.pen();
    p.setColor(Qt::blue);
    p.setWidth(10);
    painter.setPen(p);

    for ( int i = 0; i < 3; ++i){
        painter.drawPoint(points[i]);
    }
    for ( int i = 0; i < n; i+=3){
        Circle cir(points[i], points[i+1],points[i+2]);
        painter.drawEllipse(cir.center, qreal(cir.r), qreal(cir.r));
        qDebug() << "r: " << cir.r << " I am here baby";
    }
//    for ( int i = 0; i < n-2; i+=3 ){
//        Circle cir = circleEq(points[i], points[i+1]);
//    }

}


void ga18_smallestEnclosingDisk::runNaiveAlgorithm()
{

}


void ga18_smallestEnclosingDisk::minDisk(
        std::vector<QPointF> points ){
    if (points.size() < 3){
        //TODO special case
        return;
    }

}


void ga18_smallestEnclosingDisk::minDisk(
        std::vector<QPointF> points, QPointF p1){
    (void)points;
    (void)p1;
}


void ga18_smallestEnclosingDisk::minDisk(
        std::vector<QPointF> points, QPointF p1, QPointF p2)
{
    (void)points;
    (void)p1;
    (void)p2;
}


Circle::Circle( const QPointF p1, const QPointF p2, const QPointF p3 )
{
    double ma = ( p2.y() - p1.y() ) / ( p2.x() - p1.x() );
    double mb = ( p3.y() - p2.y() ) / ( p3.x() - p2.x() );
    double cenX = ( ma * mb * ( p1.y() - p3.y() )
            + mb * ( p1.x() + p2.x() )
            - ma * ( p2.x() + p3.x() ) )
            / ( 2.0 * ( mb - ma ) );
    double cenY = -1 * ( cenX - ( p1.x() + p2.x() ) / 2.0 ) / ma
            + ( p1.y() + p2.y() ) / 2.0;
    center = QPointF( cenX, cenY );
    r = my_utils::distance(center,p1);
    area = r*r*pi__;
}


Circle::Circle( const QPointF p1, const QPointF p2)
{
    center = (p1+p2)/2;
    r = my_utils::distance(center,p1);
    area = r*r*pi__;
}


namespace my_utils{

    double distance(QPointF a, QPointF b)
    {
        if((a.x() == b.x()) && (a.y() == b.y())) return 0;
        return sqrt(pow((a.x() - b.x()),2) + pow(a.y() - b.y() ,2));
    }

}

/*  BRUTE FORCE:
    - Theorem: For any finite set of points in
    general position, the smallest enclosing
    disk either has at least three points on its
    boundary, or two points which form a
    diameter. If there are three points, they
    subdivide the circle into three arcs of
    length no more than ' each. Prove !

    - This immediately implies a O(n4) algorithm
*/


/*
#include <vector>;
std::vector<int> _points;
struct circle{
    double whatever;
    double whatever2;
};
void bruteForce(){
    for( int i = 0; i < _points.size()-2; ++i){
        for (int j = i+1; j < _points.size()-1; ++j) {
            for (int k = j+1; k < _points.size(); ++k) {
                circleEquation = circleByDots(_points[i], _points[j], _points[k]);
                if (diskEnclosesAllPoints(&circleEquation)){
                    double area = circleArea(circleEquation);
                    if ( area > minArea ){
                        minArea = area;
                        minCircle = circleEquation;
                    }
                }
            }
        }
    }
}
*/

