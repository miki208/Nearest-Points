#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>

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

private:
    void animationButtonsSetEnabled(bool value);
    void algorithmOptionsSetEnabled(bool value);
    void makeNewAlgotirhm(std::string filename = "");

    enum Practice_AlgorithmType {
        EMPTY_PRACTICE = 0,
        DRAW_POLYGON,
        SWEEP_LINE,
        MAX_PRACTICE = 99
    };
    enum Projects_AlgorithmType {
        EMPTY_PROJECTS = 100,
        /* Ovde se ubacuju enum-i za studentske projekte
         * (nije potrebno navoditi konkretne brojeve)
        */
        MAX_PROJECTS = 199
    };

private slots:
    void on_algorithmType_currentIndexChanged(int index);
    void on_importDataFromFile_clicked();
    void on_generateRandomData_clicked();
    void on_clean_clicked();
    void on_start_clicked();
    void on_pause_clicked();
    void on_next_clicked();
    void on_restart_clicked();

public slots:
    void on_animationFinished();

private:
    Ui::MainWindow *ui;
    RenderArea *_renderArea;
    AlgorithmBase* _pAlgorithm;
    int _delayMs;
    std::string _filename;
};

#endif // MAINWINDOW_H
