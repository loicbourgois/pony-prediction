#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  simulation()
{
  ui->setupUi(this);
  simulation.loadRaces(QDate(2013, 01, 1), QDate(2013, 01, 31));
  simulation.prepareData();
  simulation.loadBrains(5, 2, 20);
  simulation.start();
}

MainWindow::~MainWindow()
{
  delete ui;
}
