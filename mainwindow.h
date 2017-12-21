#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>

#include "timemeasurementthread.h"

/*QChart*/
#include <QtCharts/QLineSeries>
QT_CHARTS_USE_NAMESPACE

#include "renderarea.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum Practice_AlgorithmType {
        EMPTY_PRACTICE = 0,
        DRAW_POLYGON,
        SWEEP_LINE,
        GRAHAM_SCAN,
        LINE_SEGMENT_INTERSECTION,
        DCEL_DEMO,
        TRIANGULATION,
        MAX_PRACTICE = 99
    };
    enum Projects_AlgorithmType {
        EMPTY_PROJECTS = 100,
        NEAREST_POINTS,
        INCREMENTAL_INSERTION,
        QUICK_HULL,
        POINT_ROBOT_SHORTEST_PATH,
        QUADTREE,
        /* Ovde se ubacuju enum-i za studentske projekte
         * (nije potrebno navoditi konkretne brojeve)
        */
        MAX_PROJECTS = 199
    };
private:
    void animationButtonsSetEnabled(bool value);
    void algorithmOptionsSetEnabled(bool value);
    void makeNewAlgotirhm(std::string filename = "");

private slots:
    void on_algorithmType_currentIndexChanged(int index);
    void on_importDataFromFile_clicked();
    void on_generateRandomData_clicked();
    void on_clean_clicked();
    void on_start_clicked();
    void on_pause_clicked();
    void on_next_clicked();
    void on_restart_clicked();

    void on_startMeasurement_clicked();
    void on_lineSeriesChange(double dim, double optimal, double naive);

public slots:
    void on_animationFinished();

private:
    Ui::MainWindow *ui;
    RenderArea *_renderArea;
    AlgorithmBase* _pAlgorithm;
    int _delayMs;
    std::string _filename;

    QLineSeries* _naiveSeries;
    QLineSeries* _optimalSeries;

    TimeMeasurementThread* _mThread;
};

#endif // MAINWINDOW_H
