#include "renderarea.h"
#include <iostream>

#include "mainwindow.h"
#include "algorithmbase.h"

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent), _pAlgorithmBase{nullptr}
{}

void RenderArea::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.drawRect(0,0, painter.window().width()-1, painter.window().height()-1);

    if (_pAlgorithmBase)
    {
        _pAlgorithmBase->drawAlgorithm(painter);
    }
}

void RenderArea::setPAlgorithmBase(AlgorithmBase *pAlgorithmBase)
{
    _pAlgorithmBase = pAlgorithmBase;
}
