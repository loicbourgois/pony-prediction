#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>
#include <core/util.hpp>
#include <brain/brain.hpp>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  simulation(),
  pathToBestBrain("C:/Users/Loic/pony-prediction/saves/best.brain")
{
  //
  ui->setupUi(this);
  QSplitterHandle *handle = ui->splitter->handle(1);
  QVBoxLayout *layout = new QVBoxLayout(handle);
  layout->setSpacing(0);
  layout->setMargin(0);
  QFrame *line = new QFrame(handle);
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Raised);
  layout->addWidget(line);
  handle = ui->splitter->handle(2);
  layout = new QVBoxLayout(handle);
  layout->setSpacing(0);
  layout->setMargin(0);
  line = new QFrame(handle);
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Raised);
  layout->addWidget(line);
  ui->plainTextEditLogs->setMaximumBlockCount(1000);
  //
  Util::init(this);
  QObject::connect(this,
                   SIGNAL(newLog(QString)),
                   this->ui->plainTextEditLogs,
                   SLOT(appendPlainText(QString)));
  QObject::connect(ui->doubleSpinBoxMutationFrequency,
                   SIGNAL(valueChanged(double)),
                   this,
                   SLOT(onMutationFrequencyChanged(double)));

  QObject::connect(ui->doubleSpinBoxMutationIntensity,
                   SIGNAL(valueChanged(double)),
                   this,
                   SLOT(onMutationIntensityChanged(double)));
  QObject::connect(ui->pushButtonSaveBestBrain,
                   SIGNAL(clicked()),
                   this,
                   SLOT(onSaveBestBrain()));
  ui->doubleSpinBoxMutationFrequency->setValue(0.2f);
  ui->doubleSpinBoxMutationIntensity->setValue(0.2f);
  //
  simulation.loadRaces(QDate(2013, 01, 01), QDate(2013, 01, 31));
  simulation.prepareData();
  simulation.loadBrains(5, 3, 20);
  simulation.start();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::addLog(const QString & log)
{
  emit newLog(log);
}

void MainWindow::onMutationFrequencyChanged(double value)
{
  Brain::setMutationFrequency(value);
}

void MainWindow::onMutationIntensityChanged(double value)
{
  Brain::setMutationIntensity(value);
}

void MainWindow::onSaveBestBrain()
{
  Brain::saveBestBrain(pathToBestBrain);
}
