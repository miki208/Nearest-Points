#include "ga18_smallestenclosingdisk.h"

#include <limits>
#include <QPainter>
#include <QDebug>


MinEnclosingCircle::MinEnclosingCircle(
        QWidget *pRenderer, int delayMs, std::string filename )
        :AlgorithmBase (pRenderer, delayMs), n(0),
          minCircle(QPointF(-1,-1), -1, std::numeric_limits<double>::max())
{
    std::vector<QPoint> pointInt;
    if(filename == "")
        pointInt = generateRandomPoints();
    else
        pointInt = readPointsFromFile(filename);
    for ( auto p : pointInt){
        points.emplace_back(p);
    }

    minCircle = Circle(QPointF(-1,-1), -1, std::numeric_limits<double>::max());
}


MinEnclosingCircle::MinEnclosingCircle(
        QWidget *pRenderer, int delayMs, std::vector<QPointF> newPoints )
        :AlgorithmBase (pRenderer, delayMs), n(0),
          minCircle(QPointF(-1,-1), -1, std::numeric_limits<double>::max())
{
    for ( auto p : newPoints){
        points.emplace_back(p);
    }
}


void MinEnclosingCircle::runAlgorithm()
{
//    minDisk(points);
    while ( (unsigned)n < points.size() ){
        AlgorithmBase_updateCanvasAndBlock();
        n++;
    }

//    runNaiveAlgorithm();
    emit animationFinished();
}


void MinEnclosingCircle::drawAlgorithm(QPainter &painter) const
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QPen p = painter.pen();
    p.setColor(Qt::black);
    p.setWidth(5);
    painter.setPen(p);
//    points.at(0);

    for ( int i = 0; i < 3; ++i){
        painter.drawPoint(points[i]);
    }

    if ( minCircle.r > 0 ){
        painter.drawEllipse(minCircle.center, minCircle.r, minCircle.r);
    }

    for ( int i = 0; i < n; i+=3){
        Circle cir(points[i], points[i+1],points[i+2]);
        painter.drawEllipse(cir.center, qreal(cir.r), qreal(cir.r));
        qDebug() << "r: " << cir.r << " I am here baby";
    }
}

void MinEnclosingCircle::minDisk(
        std::vector<QPointF> pts ){
    if ( pts.size() < 2 ){
        return;
    }
    if (pts.size() == 2){
        minCircle = Circle(pts[0], pts[1]);
        AlgorithmBase_updateCanvasAndBlock();
        return;
    }
    if (pts.size() == 3){
        minCircle = Circle(pts[0], pts[1], pts[2]);
        AlgorithmBase_updateCanvasAndBlock();
        return;
    }
    Circle currMinCircle(pts[0], pts[1]);
    for ( QPointF p : pts ){
        // TODO logic
        (void)p;
    }
}


void MinEnclosingCircle::minDisk(
        std::vector<QPointF> pts, QPointF p1){
    (void)pts;
    (void)p1;
    // TODO logic
}


void MinEnclosingCircle::minDisk(
        std::vector<QPointF> pts, QPointF p1, QPointF p2)
{
    (void)pts;
    (void)p1;
    (void)p2;
    // TODO logic
}


void MinEnclosingCircle::runNaiveAlgorithm()
{
    clearResult();
    for ( unsigned i = 0; i < points.size() - 1; ++i ){
        for (unsigned j = i+1; j < points.size(); ++j) {
            Circle currCircle(points[i], points[j]);
            if ( currCircle.area < minCircle.area && circleEnclosesPoints(currCircle) ){
                minCircle.area = currCircle.area;
                minCircle.r = currCircle.r;
                minCircle.center.rx() = currCircle.center.x();
                minCircle.center.ry() = currCircle.center.y();
            }
        }
    }
    for ( unsigned i = 0; i < points.size()-2; ++i){
        for (unsigned j = i+1; j < points.size()-1; ++j) {
            for (unsigned k = j+1; k < points.size(); ++k) {
                Circle currCircle = Circle(points[i], points[j], points[k]);
                if ( currCircle.area < minCircle.area && circleEnclosesPoints(currCircle) ){
                    minCircle.area = currCircle.area;
                    minCircle.r = currCircle.r;
                    minCircle.center.rx() = currCircle.center.x();
                    minCircle.center.ry() = currCircle.center.y();
                }
            }
        }
    }
}


Circle MinEnclosingCircle::result() const
{
    return minCircle;
}


void MinEnclosingCircle::clearResult()
{
    minCircle = Circle(QPointF(-1,-1), 0, std::numeric_limits<double>::max() );
//    minCircle.center.rx() = -1;
//    minCircle.center.ry() = -1;
//    minCircle.r = 0;
//    minCircle.area = std::numeric_limits<double>::max();
}



Circle::Circle( const QPointF p1_, const QPointF p2_, const QPointF p3_ )
{
    // if either line is vertical, the slope is inf, so we rearrange the points
    QPointF p1 = QPointF(p1_);
    QPointF p2 = QPointF(p2_);
    QPointF p3 = QPointF(p3_);
    if ( p2_.x() - p1_.x() == 0 ){
        std::swap(p2, p3);
    }
    else if ( p3_.x() - p2_.x() == 0 ){
        std::swap(p1, p2);
    }

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

Circle& Circle::operator=( const Circle other )
{
    center.rx() = other.center.x();
    center.ry() = other.center.y();
    r = other.r;
    area = other.area;
    return *this;
}

namespace my_utils{

    double distance(QPointF a, QPointF b)
    {
        if((a.x() == b.x()) && (a.y() == b.y())) return 0;
        return sqrt(pow((a.x() - b.x()),2) + pow(a.y() - b.y() ,2));
    }
}


bool MinEnclosingCircle::circleEnclosesPoints(Circle currCircle)
{
    for ( auto p : points ){
        if ( my_utils::distance(currCircle.center, p) > currCircle.r ){
            return false;
        }
    }
    return true;
}
