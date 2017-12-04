#ifndef GA05_TRIANGULATION_H
#define GA05_TRIANGULATION_H

#include "algorithmbase.h"
#include "ga04_dcel.h"

class Triangulation : public AlgorithmBase
{
public:
    Triangulation(QWidget *pRenderer, int delayMs, std::string filename);

    // AlgorithmBase interface
public:
    void runAlgorithm();
    void drawAlgorithm(QPainter &painter) const;
    void runNaiveAlgorithm();

private:
    DCEL _polygon;
};

#endif // GA05_TRIANGULATION_H
