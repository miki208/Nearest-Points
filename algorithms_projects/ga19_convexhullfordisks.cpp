#include "ga19_convexhullfordisks.h"
namespace ConvexHullForDisks {
ConvexHullForDisks::ConvexHullForDisks(QWidget *pRender, int delayMs, std::string fileName)
    :AlgorithmBase (pRender, delayMs)
{
//    this->disk1 = new Disk(new QPointF(100, 100), 10.0);
    //    }
        Disk* disk1 = new Disk(new QPointF(100, 100), 10.0);

        Disk* disk2 = new Disk(new QPointF(200, 200), 20.0);


        this->disks.push_back(disk1);
        this->disks.push_back(disk2);
//        this->bridges.push_back(disk1->tangent(disk2));
//        this->bridges.push_back(disk2->tangent(disk1));

}

void ConvexHullForDisks::runAlgorithm()
{
    for(int i=0; i< 10; i++){
        this->disks[0]->radius += 10.0;
        this->disks[1]->radius += 15.0;

        this->bridges.clear();
        this->bridges.push_back(disks[0]->tangent(disks[1]));
//        this->bridges.push_back(disks[1]->tangent(disks[0]));
        AlgorithmBase_updateCanvasAndBlock();
    }
//    AlgorithmBase_updateCanvasAndBlock();

}

void ConvexHullForDisks::drawAlgorithm(QPainter &painter) const
{
    QPen p = painter.pen();
    p.setWidth(1);
    p.setColor(QColor::fromRgb(128, 128, 128));
    painter.setPen(p);

//    painter.drawEllipse();
    std::for_each(this->disks.begin(), this->disks.end(), [&painter](Disk* disk){painter.drawEllipse(*disk->center, disk->radius, disk->radius);});
    std::for_each(this->bridges.begin(), this->bridges.end(), [&painter](QLineF* bridge) {painter.drawLine(*bridge);});
//    painter.drawEllipse(*(this->disks->center), this->disk1->radius, this->disk1->radius);
//    painter.drawEllipse(*(this->disk2->center), this->disk2->radius, this->disk2->radius);
//    painter.drawLine(*(this->tangenta));

}

void ConvexHullForDisks::runNaiveAlgorithm()
{

}

Disk::Disk(QPointF* center, double radius)
{
    this->center = center;
    this->radius = radius;
}

QLineF* Disk::tangent(Disk* other_disk) {
    double x1 = this->center->x();
    double y1 = this->center->y();
    double r = this -> radius;

    double x2 = other_disk->center->x();
    double y2 = other_disk->center->y();
    double R = other_disk->radius;

    double gama = - atan((y2 - y1)/(x2 - x1));
    double beta = asin((R - r)/sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
    double alpha = gama - beta;

    double x3 = x1 + r * cos(PI/2 - alpha);
    double y3 = y1 + r * sin(PI/2 - alpha);
    double x4 = x2 + R * cos(PI/2 - alpha);
    double y4 = y2 + R * sin(PI/2 - alpha);

    return new QLineF(x3, y3, x4, y4);
}
}
