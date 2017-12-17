#include <QPainter>

#include "ga05_quickhull.h"
#include "utils.h"


//-----------------------------------------------------------------------------
// QuickHull
//-----------------------------------------------------------------------------
QuickHull::QuickHull(QWidget *pRenderer, int delayMs, std::string filename, int inputSize)
    : ConvexHull(pRenderer, delayMs, filename, inputSize)
{}

//-----------------------------------------------------------------------------
// runAlgorithm
//-----------------------------------------------------------------------------
void QuickHull::runAlgorithm()
{
    emit animationFinished();
}

//-----------------------------------------------------------------------------
// drawAlgorithm
//-----------------------------------------------------------------------------
void QuickHull::drawAlgorithm(QPainter &painter) const
{
    (void)painter;
}

//-----------------------------------------------------------------------------
// runNaiveAlgorithm
//-----------------------------------------------------------------------------
void QuickHull::runNaiveAlgorithm()
{
    ConvexHull::runNaiveAlgorithm();
}

//-----------------------------------------------------------------------------
// _whichSide
//-----------------------------------------------------------------------------
bool QuickHull::_whichSide(QPoint x, QPoint x1, QPoint x2)
{
    return ((x.x() - x1.x())*(x2.y()-x1.y())-(x.y()-x1.y())*(x2.x()-x1.x()) < 0) ? false : true;
}

//-----------------------------------------------------------------------------
// getConvexHull
//-----------------------------------------------------------------------------
std::vector<QPoint> QuickHull::getConvexHull() const
{
    return _convexHull;
}

//-----------------------------------------------------------------------------
// getConvexHullTest
//-----------------------------------------------------------------------------
std::vector<QPoint> QuickHull::getConvexHullTest() const
{
    return ConvexHull::convexHullTest();
}
