#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <core/util.hpp>
#include <brain/brain.hpp>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  simulation(),
  pathToSaves("C:/Users/Loic/pony-prediction/saves")
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
  QObject::connect(ui->pushButtonLoad,
                   SIGNAL(clicked()),
                   this,
                   SLOT(onLoad()));
  QObject::connect(ui->pushButtonPlay,
                   SIGNAL(clicked()),
                   this,
                   SLOT(onPlay()));
  QObject::connect(ui->pushButtonPause,
                   SIGNAL(clicked()),
                   this,
                   SLOT(onPause()));
  QObject::connect(ui->pushButtonReset,
                   SIGNAL(clicked()),
                   this,
                   SLOT(onReset()));
  ui->doubleSpinBoxMutationFrequency->setValue(0.2f);
  ui->doubleSpinBoxMutationIntensity->setValue(0.2f);
  //
  simulation.loadRaces(QDate(2013, 01, 01), QDate(2013, 01, 31));
  simulation.prepareData();
  simulation.loadBrains(5, 4, 20);
  onPlay();
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
  onPause();
  QDir dir(pathToSaves);
  if (!dir.exists()) {
    dir.mkpath(pathToSaves);
  }
  QString path = QFileDialog::getSaveFileName(this, "Save", pathToSaves + "/best", "Save (*.brain)");
  if(path.size())
  {
    Brain::saveBestBrain(path);
  }
  onPlay();
}

void MainWindow::onLoad()
{
  onPause();
  QDir dir(pathToSaves);
  if (!dir.exists()) {
    dir.mkpath(pathToSaves);
  }
  QString path = QFileDialog::getOpenFileName(this, "Open", pathToSaves, "Save (*.brain)");
  if(path.size())
  {
    simulation.loadBrains(path, 5);
  }
  onPlay();
}

void MainWindow::onPlay()
{
  ui->pushButtonPlay->setEnabled(false);
  ui->pushButtonPause->setEnabled(true);
  simulation.play();
}

void MainWindow::onPause()
{
  ui->pushButtonPlay->setEnabled(true);
  ui->pushButtonPause->setEnabled(false);
  simulation.pause();
}

void MainWindow::onReset()
{

}
