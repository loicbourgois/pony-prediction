#include "main-window.hpp"
#include "ui_main-window.h"
#include <QDate>
#include <core/util.hpp>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  simulation()
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
  //
  Util::init(this);
  QObject::connect(this, SIGNAL(newLog(QString)), this->ui->textEditLogs, SLOT(append(QString)));
  //
  simulation.loadRaces(QDate(2013, 01, 1), QDate(2013, 01, 31));
  simulation.prepareData();
  simulation.loadBrains(5, 3, 20);
  simulation.start();

}

MainWindow::~MainWindow()
{
  delete ui;
}
