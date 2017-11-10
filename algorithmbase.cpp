#include "algorithmbase.h"

#include "animationthread.h"
#include <fstream>

AlgorithmBase::AlgorithmBase(QWidget *pRender, int delayMs)
    : QObject{}, _pRenderer{pRender}, _destroyAnimation{false}, _delayMs{delayMs}, _timerId{INVALID_TIMER_ID}, _semaphore{0}, _pThread{nullptr}
{}

void AlgorithmBase::startAnimation()
{
    delete _pThread;
    _pThread = new AnimationThread{this};
    _pThread->start();
    _timerId = startTimer(_delayMs);
    _pRenderer->update();
}

void AlgorithmBase::pauseOrContinueAnimation()
{
    if (INVALID_TIMER_ID != _timerId)
    {
        killTimer(_timerId);
        _timerId = INVALID_TIMER_ID;
    }
    else
        _timerId = startTimer(_delayMs);
}

void AlgorithmBase::changeDelay(int delayMs)
{
    if (INVALID_TIMER_ID != _timerId)
        killTimer(_timerId);
    _delayMs = delayMs;
    _timerId = startTimer(_delayMs);
}

void AlgorithmBase::resetAnimation()
{
    _destroyAnimation = true;
    _semaphore.release();
    _pThread->quit();
    _pThread->wait();

    delete _pThread;
    _pThread = nullptr;
    _destroyAnimation = false;
}

void AlgorithmBase::nextStep()
{
    if (INVALID_TIMER_ID != _timerId)
    {
        killTimer(_timerId);
        _timerId = INVALID_TIMER_ID;
    }
    _semaphore.release();
}

bool AlgorithmBase::updateCanvasAndBlock()
{
    _pRenderer->update();
    _semaphore.acquire();
    return _destroyAnimation;
}

void AlgorithmBase::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    _semaphore.release();
}

std::vector<QPoint> AlgorithmBase::generateRandomPoints(int pointsNum)
{
    srand(static_cast<unsigned>(time(0)));

    int xMax = _pRenderer->width()-DRAWING_BORDER;
    int yMax = _pRenderer->height() - DRAWING_BORDER;

    int xMin = DRAWING_BORDER;
    int yMin = DRAWING_BORDER;

    std::vector<QPoint> randomPoints;

    int xDiff = xMax-xMin;
    int yDiff = yMax-yMin;
    for(int i=0; i < pointsNum; i++)
        randomPoints.emplace_back(xMin + rand()%xDiff, yMin + rand()%yDiff);

    return randomPoints;
}

std::vector<QPoint> AlgorithmBase::readPointsFromFile(std::string fileName)
{
    std::ifstream inputFile(fileName);
    std::vector<QPoint> points;
    int x, y;
    while(inputFile >> x >> y)
    {
        points.emplace_back(x, y);
    }
    return points;
}

std::vector<QLineF> AlgorithmBase::generateRandomLines(int linesNum)
{
    srand(static_cast<unsigned>(time(0)));

    int xMax = _pRenderer->width()-DRAWING_BORDER;
    int yMax = _pRenderer->height() - DRAWING_BORDER;

    int xMin = DRAWING_BORDER;
    int yMin = DRAWING_BORDER;

    std::vector<QLineF> randomLines;

    int xDiff = xMax-xMin;
    int yDiff = yMax-yMin;
    for(int i=0; i < linesNum; i++)
    {
        //Hocemo da p1 bude upper point, a p2 lower point
        QPointF p1 = QPointF(xMin + rand()%xDiff, yMin + rand()%yDiff);
        QPointF p2 = QPointF(xMin + rand()%xDiff, yMin + rand()%yDiff);
        if(p1.y() < p2.y())
            randomLines.emplace_back(p2, p1);
        else
            randomLines.emplace_back(p1, p2);
    }
    return randomLines;
}
