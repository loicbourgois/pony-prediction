#pragma once

#include <QMainWindow>
#include "core/simulation.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  //public slots:
    void addLog(const QString & log);
  signals:
    void newLog(const QString & log);

  private:
    Ui::MainWindow *ui;
    Simulation simulation;
};
