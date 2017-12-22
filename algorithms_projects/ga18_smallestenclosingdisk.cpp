#include "ga18_smallestenclosingdisk.h"

#include <QPainter>

ga18_smallestEnclosingDisk::ga18_smallestEnclosingDisk(QWidget *pRenderer, int delayMs, std::string filename )
    :AlgorithmBase (pRenderer, delayMs), n(0)
{
    if(filename == "")
        points = generateRandomPoints();
    else
        points = readPointsFromFile(filename);
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
//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QPen p = painter.pen();
    p.setColor(Qt::green);
    painter.setPen(p);

    for ( int i = 0; i < n-1; ++i){
        painter.drawLine(points[i], points[i+1]);
    }
}

void ga18_smallestEnclosingDisk::runNaiveAlgorithm()
{

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

