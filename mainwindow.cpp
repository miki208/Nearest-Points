#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>
#include <fstream>

#include "algorithmbase.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _pAlgorithm{nullptr}, _delayMs{500}
{
    ui->setupUi(this);

    setWindowTitle("Geometrijski algoritmi @ MATF");

    ui->algorithmType->addItem("", 0);

    ui->importDataFromFile->setEnabled(false);
    ui->generateRandomData->setEnabled(false);

    QBoxLayout* renderBoxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    _renderArea = new RenderArea(this);
    renderBoxLayout->addWidget(_renderArea);
    ui->tab->setLayout(renderBoxLayout);

    animationButtonsSetEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animationButtonsSetEnabled(bool value)
{
    ui->start->setEnabled(value);
    ui->pause->setEnabled(value);
    ui->next->setEnabled(value);
    ui->restart->setEnabled(value);
}

void MainWindow::algorithmOptionsSetEnabled(bool value)
{
    ui->algorithmType->setEnabled(value);
    ui->importDataFromFile->setEnabled(value);
    ui->generateRandomData->setEnabled(value);
    ui->clean->setEnabled(value);
}

void MainWindow::makeNewAlgotirhm(std::string filename)
{
    delete _pAlgorithm;
    _pAlgorithm = nullptr;

    _filename = filename;

    switch (ui->algorithmType->currentIndex())
    {
        case 0:
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
    if(index != 0)
    {
        ui->importDataFromFile->setEnabled(true);
        ui->generateRandomData->setEnabled(true);
    }
    else
    {
        ui->importDataFromFile->setEnabled(false);
        ui->generateRandomData->setEnabled(false);
    }
    animationButtonsSetEnabled(false);
}

void MainWindow::on_importDataFromFile_clicked()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Fajl koji sadrzi koordinate tacaka"), ".", "*.*");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Fajl koji sadrzi koordinate tacaka"), "../GeometryAlgorithms/inputs", "*.*");
    if(fileName.isEmpty())
        return;

    makeNewAlgotirhm(fileName.toStdString());
    animationButtonsSetEnabled(true);
}

void MainWindow::on_generateRandomData_clicked()
{
    makeNewAlgotirhm("");
    animationButtonsSetEnabled(true);
}

void MainWindow::on_clean_clicked()
{
    ui->algorithmType->setCurrentIndex(0);
    animationButtonsSetEnabled(false);
    _renderArea->update();
}

void MainWindow::on_start_clicked()
{
    ui->start->setEnabled(false);
    algorithmOptionsSetEnabled(false);

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
    ui->start->setEnabled(false);
    ui->pause->setEnabled(true);
    ui->next->setEnabled(true);

    if (_pAlgorithm)
    {
        _pAlgorithm->resetAnimation();
        makeNewAlgotirhm(_filename);
        _pAlgorithm->startAnimation();
    }
}

void MainWindow::on_animationFinished()
{
    ui->algorithmType->setEnabled(true);
    ui->clean->setEnabled(true);
    animationButtonsSetEnabled(false);
    ui->restart->setEnabled(true);
}
