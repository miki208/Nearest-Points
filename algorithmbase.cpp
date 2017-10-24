#include "algorithmbase.h"

#include "animationthread.h"
#include <fstream>

AlgorithmBase::AlgorithmBase(QWidget *pRender, int delayMs)
    : QObject{}, _pRenderer{pRender}, _delayMs{delayMs}, _timerId{INVALID_TIMER_ID}, _semaphore{0}, _destroyAnimation{false}, _pThread{nullptr}
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

std::vector<QPoint> AlgorithmBase::generateRandomPoints()
{
    srand((unsigned)time(0));
    int xMax = 1000;
    int yMax = 450;

    int xMin = 10;
    int yMin = 10;
    std::vector<QPoint> randomPoints;

    for(int i=0; i < 20; i++)
        randomPoints.emplace_back(xMin + rand()%(xMax-xMin), yMin + rand()%(yMax-yMin));

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
