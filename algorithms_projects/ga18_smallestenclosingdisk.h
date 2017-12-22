#ifndef GA18_SMALLESTENCLOSINGDISK_H
#define GA18_SMALLESTENCLOSINGDISK_H

#include "algorithmbase.h"
#include <vector>

class ga18_smallestEnclosingDisk : public AlgorithmBase
{
public:
    ga18_smallestEnclosingDisk(QWidget *pRenderer, int delayMs, std::string filename);

public:
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

private:
    int n;
    std::vector<QPoint> points;

};

#endif // GA18_SMALLESTENCLOSINGDISK_H
