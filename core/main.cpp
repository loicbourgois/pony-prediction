#include "ui/main-window.hpp"
#include <time.h>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
  QTime time = QTime::currentTime();
  unsigned int seed = time.msec();
  qDebug() << seed;
  qsrand(seed);
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
