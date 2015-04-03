#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    simulation()
{
    ui->setupUi(this);
    simulation.loadRaces(QDate(2015, 01, 01), QDate(2015, 01, 02));
    simulation.prepareData();
    simulation.loadBrains(1, 3, 20);
    simulation.run();
}

MainWindow::~MainWindow()
{
    delete ui;
}
