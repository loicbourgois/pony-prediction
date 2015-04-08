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
  signals:
    void newLog(const QString & log);
  private slots:
    void onMutationFrequencyChanged(double value);
    void onMutationIntensityChanged(double value);
  private:
    void addLog(const QString & log);
    Ui::MainWindow *ui;
    Simulation simulation;
};
