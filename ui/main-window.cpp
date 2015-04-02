#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    simulation()
{
    ui->setupUi(this);
    simulation.loadData(QDate(2015, 01, 01), QDate(2015, 01, 02));
}

MainWindow::~MainWindow()
{
    delete ui;
}
