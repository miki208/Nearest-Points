#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLineEdit>
#include <iostream>
#include <fstream>

/*QChart*/
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

#include "algorithmbase.h"
#include "algorithms_practice/ga01_sweepline.h"
#include "algorithms_practice/ga00_drawpolygon.h"
#include "algorithms_practice/ga02_grahamscan.h"
#include "algorithms_practice/ga03_linesegmentintersection.h"
#include "algorithms_practice/ga04_dceldemo.h"
#include "algorithms_practice/ga05_triangulation.h"

#include "algorithms_projects/ga03_nearestpoints.h"
#include "algorithms_projects/ga05_incrementalinsertion.h"
#include "algorithms_projects/ga05_quickhull.h"
#include "algorithms_projects/ga15_pointrobotshortestpath.h"
#include "algorithms_projects/ga16_quadtree.h"
#include "algorithms_projects/ga18_smallestenclosingdisk.h"
#include "algorithms_projects/ga21_fixedradiuscircle.h"
#include "algorithms_projects/ga19_convexhullfordisks.h"
#include "algorithms_projects/ga11_intervalsearchtree.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _pAlgorithm{nullptr}, _delayMs{500}     // (default: 500)
{
    ui->setupUi(this);
    setWindowTitle("Geometrijski algoritmi @ MATF");

    /* Algorithm type */
    ui->algorithmType->addItem("SA ČASOVA VEŽBI:", QVariant(EMPTY_PRACTICE));
    ui->algorithmType->addItem("Demonstacija iscrtavanja", QVariant(DRAW_POLYGON));
    ui->algorithmType->addItem("Brišuća prava (mini demo)", QVariant(SWEEP_LINE));
    ui->algorithmType->addItem("Konveksni omotač (Graham scan)", QVariant(GRAHAM_SCAN));
    ui->algorithmType->addItem("Preseci duži", QVariant(LINE_SEGMENT_INTERSECTION));
    ui->algorithmType->addItem("DCEL (demo)", QVariant(DCEL_DEMO));
    ui->algorithmType->addItem("Triangulacija", QVariant(TRIANGULATION));

    ui->algorithmType->insertSeparator(MAX_PRACTICE);

    ui->algorithmType->addItem("STUDENTSKI PROJEKTI:", QVariant(EMPTY_PROJECTS));
    /* Ovde se ubacuju opcije za izbor studentskih projekata [START]*/
    ui->algorithmType->addItem("Određivanje dve najbliže tačke u ravni", QVariant(NEAREST_POINTS));
    ui->algorithmType->addItem("Konveksni omotač (Incremental insertion)", QVariant(INCREMENTAL_INSERTION));
    ui->algorithmType->addItem("Konveksni omotač (QuickHull)", QVariant(QUICK_HULL));
    //ui->algorithmType->addItem("Odredjivanje najkraceg puta sa obilazenjem prepreka za tackastog robota", QVariant(POINT_ROBOT_SHORTEST_PATH));
    ui->algorithmType->addItem("Algoritam za određivanje kolizija korišćenjem strukture Quadtree", QVariant(QUADTREE));
    ui->algorithmType->addItem("Odredjivanje diska najmanjeg poluprecnika koji pokriva sve tacke u ravni", QVariant(SMALLEST_ENCLOSING_CIRCLE));
    ui->algorithmType->addItem("Pozicioniranje kruga fiksnog precnika u ravni - maksimizovanje tacaka u njegovoj unutrasnjosti", QVariant(FIXEDRADIUSCIRCLE));    
    ui->algorithmType->addItem("Convex hull for disks", QVariant(CONVEXHULLFORDISKS));
    ui->algorithmType->addItem("Stablo pretrage duzi", QVariant(INTERVAL_SEARCH_TREE));
    /* Ovde se ubacuju opcije za izbor studentskih projekata [END]*/
    ui->algorithmType->insertSeparator(MAX_PROJECTS);

    /* Render area setup */
    QBoxLayout* renderBoxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    _renderArea = new RenderArea(this);
    renderBoxLayout->addWidget(_renderArea);
    ui->tab->setLayout(renderBoxLayout);

    /* Chart */
    _optimalSeries = new QLineSeries();
    _naiveSeries = new QLineSeries();

    QChart *chart = new QChart();
    _optimalSeries->append(0,0);
    _naiveSeries->append(0,0);

    _optimalSeries->setName("Optimalni algoritam");
    _naiveSeries->setName("Naivni algoritam");

    chart->addSeries(_optimalSeries);
    chart->addSeries(_naiveSeries);

    chart->legend()->show();

    chart->createDefaultAxes();
    chart->setTitle("Poredjenje efikasnosti");

    if(chart->axisX())
        chart->axisX()->setRange(0, X_MAX_VAL);

    if(chart->axisY())
        chart->axisY()->setRange(0, Y_MAX_VAL);

    // Same formatting
    chart->setBackgroundVisible(false);
    chart->setPlotAreaBackgroundVisible(true);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QBoxLayout* chartBoxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    chartBoxLayout->addWidget(chartView);

    ui->tab_3->setLayout(chartBoxLayout);

    /* Initial animation speed */
    _delayMs = (12-ui->animationSpeed->value())*100;

    /* Left side options */
    ui->importDataFromFile->setVisible(false);
    ui->generateRandomData->setVisible(false);
    ui->gb2_animation->setVisible(false);
    ui->gb3_params->setVisible(false);
    ui->startMeasurement->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeNewAlgotirhm(std::string filename)
{
    delete _pAlgorithm;
    _pAlgorithm = nullptr;

    int currentIndex = ui->algorithmType->currentIndex();
    int currentAlgorithm = ui->algorithmType->itemData(currentIndex).toInt();
    int inputDim;
    bool checker;

    _filename = filename;
    _delayMs = (12-ui->animationSpeed->value())*100;
    switch (currentAlgorithm)
    {
        case EMPTY_PRACTICE:
        case EMPTY_PROJECTS:
            break;
        case DRAW_POLYGON:
             _pAlgorithm = new DrawPolygon(_renderArea, _delayMs, _filename);
             break;
        case SWEEP_LINE:
            _pAlgorithm = new SweepLine(_renderArea, _delayMs, _filename);
            break;
        case GRAHAM_SCAN:
            _pAlgorithm = new GrahamScan(_renderArea, _delayMs, _filename);
            break;
        case LINE_SEGMENT_INTERSECTION:
            inputDim = ui->gb3_params->findChild<QLineEdit*>("gui_inputDim")->text().toInt(&checker);
            if(checker)
                _pAlgorithm = new LineSegmentIntersection(_renderArea, _delayMs, _filename, inputDim);
            else
                _pAlgorithm = new LineSegmentIntersection(_renderArea, _delayMs);
            break;
        case DCEL_DEMO:
            _pAlgorithm = new DCELDemo(_renderArea, _delayMs, filename);
            break;
        case TRIANGULATION:
            _pAlgorithm = new Triangulation(_renderArea, _delayMs, filename);
            break;
        case NEAREST_POINTS:
            _pAlgorithm = new NearestPoints(_renderArea, _delayMs, _filename);
            break;
        case INCREMENTAL_INSERTION:
            _pAlgorithm = new IncrementalInsertion(_renderArea, _delayMs, _filename);
            break;
        case QUICK_HULL:
            _pAlgorithm = new QuickHull(_renderArea, _delayMs, _filename);
            break;
        case POINT_ROBOT_SHORTEST_PATH:
            _pAlgorithm = new PointRobotShortestPath(_renderArea, _delayMs, _filename);
            break;
        case QUADTREE:{
            int inputSize = 300;
            int squareSize = 30;
            //if(checker){
                squareSize = ui->gb3_params->findChild<QLineEdit*>("gui_quadtree_square_size")->text().toInt(&checker);
                inputSize = ui->gb3_params->findChild<QLineEdit*>("gui_quadtree_input_size")->text().toInt(&checker);
            //}
            Quadtree *q = new Quadtree(_renderArea, _delayMs, _filename, inputSize);
            q->setSquareSize(squareSize);
            _pAlgorithm = q;
            }
            break;
        case SMALLEST_ENCLOSING_CIRCLE:
            _pAlgorithm = new MinEnclosingCircle(_renderArea, _delayMs, _filename);
            break;
        case FIXEDRADIUSCIRCLE:
        {
            int radius = ui->gb3_params->findChild<QLineEdit*>("gui_radius")->text().toInt(&checker);
            if(!checker)
                radius = 70;
            _pAlgorithm = new FixedRadiusCircle(_renderArea, _delayMs, radius, _filename);
            break;
        }
        case CONVEXHULLFORDISKS:
            _pAlgorithm = new ConvexHullForDisks::ConvexHullForDisks(_renderArea, _delayMs, _filename);
            break;
        case INTERVAL_SEARCH_TREE:
            QString line = ui->gb3_params->findChild<QLineEdit*>("gui_interval")->text();
            QStringList fields = line.split(",");
            int x1, x2;
            bool checker1, checker2;

            if(fields.size() == 2){
                x1 = fields.at(0).toInt(&checker1);
                x2 = fields.at(1).toInt(&checker2);
            }
            else if(fields.size() != 2 || !checker1 || !checker2){
               // QErrorMessage::qtHandler()->showMessage("HELLO WORLD");
                x1 = 100;
                x2 = 300;
            }

            IntervalSearchTree *newTree = new IntervalSearchTree(_renderArea, _delayMs, _filename);
            newTree->setLine(QLineF(x1, 20, x2, 20));
            _pAlgorithm  = newTree;
            break;
    }

    if(_pAlgorithm)
    {
        _renderArea->setPAlgorithmBase(_pAlgorithm);
        connect(_pAlgorithm, SIGNAL(animationFinished()), this, SLOT(on_animationFinished()));
    }
}

void MainWindow::on_algorithmType_currentIndexChanged(int index)
{
    int currentAlgorithm = ui->algorithmType->itemData(index).toInt();

    if(currentAlgorithm != EMPTY_PRACTICE && currentAlgorithm != EMPTY_PROJECTS)
    {
        ui->importDataFromFile->setVisible(true);
        ui->generateRandomData->setVisible(true);
        ui->gb3_params->setVisible(true);
        addAditionalParams(currentAlgorithm);
    }
    else
    {
        ui->importDataFromFile->setVisible(false);
        ui->generateRandomData->setVisible(false);
        ui->gb3_params->setVisible(false);
    }
    ui->gb2_animation->setVisible(false);
}

void MainWindow::removeAdditionalParams()
{
        auto tmp = ui->gb3_params->layout();
        QLayoutItem *item;
        while ( ( item = tmp->takeAt(0) ) ){
            delete item->widget();
            delete item;
        }
        delete tmp->layout();
}

void MainWindow::addAditionalParams(int algorithmType)
{
    if ( ui->gb3_params->layout() ){
        removeAdditionalParams();
    }

    QGridLayout* additionalOptionsLayout = new QGridLayout();
    if(algorithmType == LINE_SEGMENT_INTERSECTION)
    {
            QLabel* lsi_label = new QLabel("Dimenzija ulaza: ");
            QLineEdit* lsi_text = new QLineEdit("20");
            lsi_text->setObjectName("gui_inputDim");
            additionalOptionsLayout->addWidget(lsi_label, 0, 0, 1, 1);
            additionalOptionsLayout->addWidget(lsi_text, 0, 1, 1, 1);
            ui->gb3_params->setLayout(additionalOptionsLayout);
    }
    else if (algorithmType == FIXEDRADIUSCIRCLE)
    {
        QLabel* frc_label = new QLabel("Precnik kruga: ");
        QLineEdit* frc_text = new QLineEdit("70");
        frc_text->setObjectName("gui_radius");
        additionalOptionsLayout->addWidget(frc_label, 0, 0, 1, 1);
        additionalOptionsLayout->addWidget(frc_text, 0, 1, 1, 1);
        ui->gb3_params->setLayout(additionalOptionsLayout);
    } else if (algorithmType == QUADTREE)
    {
        QLabel* label = new QLabel("Stranica kvadrata: ");
        int def = _renderArea->height() / 30;
        def = def ? def : 30;
        QLineEdit* text = new QLineEdit(QString::number(def));
        text->setObjectName("gui_quadtree_square_size");
        additionalOptionsLayout->addWidget(label, 0, 0, 1, 1);
        additionalOptionsLayout->addWidget(text, 0, 1, 1, 1);

        label = new QLabel("Broj kvadrata: ");
        text = new QLineEdit("300");
        text->setObjectName("gui_quadtree_input_size");
        additionalOptionsLayout->addWidget(label, 1, 0, 1, 1);
        additionalOptionsLayout->addWidget(text, 1, 1, 1, 1);
        ui->gb3_params->setLayout(additionalOptionsLayout);
    }
    else if (algorithmType == INTERVAL_SEARCH_TREE)
    {
        QLabel* label = new QLabel("Koordinata duzi za koju se trazi \npresek u formatu  x,y: ");
        QLineEdit* text = new QLineEdit("100,300");
        text->setObjectName("gui_interval");
        additionalOptionsLayout->addWidget(label, 0, 0, 1, 1);
        additionalOptionsLayout->addWidget(text, 1, 0, 1, 1);
        ui->gb3_params->setLayout(additionalOptionsLayout);
    }
}

void MainWindow::on_importDataFromFile_clicked()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Fajl koji sadrzi koordinate tacaka"), ".", "*.*");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Fajl koji sadrzi koordinate tacaka"), "../algorithms/input_files", "*.*");
    if(fileName.isEmpty())
        return;

    makeNewAlgotirhm(fileName.toStdString());
    ui->gb2_animation->setVisible(true);
}

void MainWindow::on_generateRandomData_clicked()
{
    makeNewAlgotirhm("");
    ui->gb2_animation->setVisible(true);
}

void MainWindow::on_clean_clicked()
{
    ui->algorithmType->setCurrentIndex(0);
    ui->gb2_animation->setVisible(false);
    ui->start->setVisible(true);
    ui->gb3_params->setVisible(false);
    ui->gb1_algorithm->setVisible(true);
    if(_pAlgorithm)
    {
        _pAlgorithm->resetAnimation();
        _pAlgorithm = nullptr;
        _renderArea->setPAlgorithmBase(nullptr);
        _renderArea->update();
    }
}

void MainWindow::on_start_clicked()
{
    ui->start->setVisible(false);
    ui->gb1_algorithm->setVisible(false);

    if(_pAlgorithm)
        _pAlgorithm->startAnimation();
}

void MainWindow::on_pause_clicked()
{
    _pAlgorithm->pauseOrContinueAnimation();
}

void MainWindow::on_next_clicked()
{
    if(_pAlgorithm)
        _pAlgorithm->nextStep();
}

void MainWindow::on_restart_clicked()
{
    ui->start->setVisible(false);
    ui->pause->setVisible(true);
    ui->next->setVisible(true);

    if (_pAlgorithm)
    {
        _pAlgorithm->resetAnimation();
        makeNewAlgotirhm(_filename);
        _pAlgorithm->startAnimation();
    }
}

void MainWindow::on_animationFinished()
{
    ui->gb1_algorithm->setVisible(true);
    ui->gb2_animation->setVisible(false);
    ui->start->setVisible(true);

}

void MainWindow::on_lineSeriesChange(double dim, double optimal, double naive)
{
    _optimalSeries->append(dim, optimal);
    _naiveSeries->append(dim, naive);
}

void MainWindow::on_startMeasurement_clicked()
{
    int currentIndex = ui->algorithmType->currentIndex();
    int currentAlgorithm = ui->algorithmType->itemData(currentIndex).toInt();

    _mThread = new TimeMeasurementThread(currentAlgorithm, MIN_DIM, STEP, MAX_DIM);
    connect(_mThread, &TimeMeasurementThread::updateChart, this, &MainWindow::on_lineSeriesChange);
    _mThread->start();
}

void MainWindow::on_animationSpeed_valueChanged(int value)
{
    if(_pAlgorithm)
    {
        _pAlgorithm->changeDelay((12-value)*100);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
        ui->startMeasurement->setVisible(false);
    }
    else{
        ui->startMeasurement->setVisible(true);
    }
}
