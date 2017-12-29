#include "timemeasurementthread.h"

#include "config.h"
#include "mainwindow.h"
#include "algorithmbase.h"
#include "algorithms_practice/ga02_grahamscan.h"
#include "algorithms_practice/ga01_sweepline.h"
#include "algorithms_practice/ga00_drawpolygon.h"
#include "algorithms_practice/ga03_linesegmentintersection.h"

#include "algorithms_projects/ga03_nearestpoints.h"
#include "algorithms_projects/ga05_incrementalinsertion.h"
#include "algorithms_projects/ga05_quickhull.h"
#include "algorithms_projects/ga16_quadtree.h"
#include "algorithms_projects/ga21_fixedradiuscircle.h"
#include "algorithms_projects/ga11_intervalsearchtree.h"

TimeMeasurementThread::TimeMeasurementThread(int algorithmType, int minValue, int step, int maxValue)
    :QThread (), _algorithmType(algorithmType), _minValue(minValue), _step(step), _maxValue(maxValue)
{
}

void TimeMeasurementThread::run()
{
    clock_t begin, end;
    double optimalTime, naiveTime;

    AlgorithmBase* pAlgorithm = nullptr;

    for(int i= _minValue; i <= _maxValue; i += _step)
    {
        switch (_algorithmType)
        {
            case MainWindow::EMPTY_PRACTICE:
            case MainWindow::EMPTY_PROJECTS:
                break;
            case MainWindow::DRAW_POLYGON:
                 pAlgorithm = new DrawPolygon(nullptr, 0, "");
                 break;
            case MainWindow::SWEEP_LINE:
                pAlgorithm = new SweepLine(nullptr, 0, "");
                break;
            case MainWindow::GRAHAM_SCAN:
                pAlgorithm = new GrahamScan(nullptr, 0, "", i);
                break;
            case MainWindow::LINE_SEGMENT_INTERSECTION:
                pAlgorithm = new LineSegmentIntersection(nullptr, 0, "", i);
                break;
            case MainWindow::NEAREST_POINTS:
                pAlgorithm = new NearestPoints(nullptr, 0, "", i);
                break;
            case MainWindow::INCREMENTAL_INSERTION:
                pAlgorithm = new IncrementalInsertion(nullptr, 0, "", i);
                break;
            case MainWindow::QUICK_HULL:
                pAlgorithm = new QuickHull(nullptr, 0, "", i);
                break;
            case MainWindow::QUADTREE:
                pAlgorithm = new Quadtree(nullptr, 0, "", i);
                break;
            case MainWindow::FIXEDRADIUSCIRCLE:
                pAlgorithm = new FixedRadiusCircle(nullptr, 0, 70, "", i);
                break;
            case MainWindow::INTERVAL_SEARCH_TREE:
                pAlgorithm = new IntervalSearchTree(nullptr, 0, "", 5000, i);
                break;
        }

        if(pAlgorithm)
        {
#ifndef SKIP_OPTIMAL
            begin = clock();
            pAlgorithm->runAlgorithm();
            end = clock();
            optimalTime = double(end - begin) / CLOCKS_PER_SEC;
#else
            optimalTime = 0;
#endif

#ifndef SKIP_NAIVE
            begin = clock();
            pAlgorithm->runNaiveAlgorithm();
            end = clock();
            naiveTime = double(end - begin) / CLOCKS_PER_SEC;
#else
            naiveTime = 0;
#endif
            emit updateChart(i, optimalTime, naiveTime);
            delete pAlgorithm;
            pAlgorithm = nullptr;
        }
    }
}
