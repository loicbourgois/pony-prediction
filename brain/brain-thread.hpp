#pragma once

#include <QThread>
#include <QVector>
#include <QObject>
#include <QMutex>
#include "core/result.hpp"
#include "brain/brain.hpp"

class BrainThread : public QThread
{
    Q_OBJECT
  public:
    BrainThread(const int &layerCount, const int &neuronsPerLayer,
                const int & inputsPerNeuronFirstLayer,
                const QVector<QVector<float> > & inputs,
                const QVector<Result> & wantedResults, const int & seed);
    BrainThread(const QString & path,
                const QVector<QVector<float> > & inputs,
                const QVector<Result> & wantedResults,
                const int & seed);
    ~BrainThread();
    void stop();
  private:
    void run();
    bool go;
    const QVector<QVector<float> > & inputs;
    const QVector<Result> & wantedResults;
    Brain brain;
    int seed;
    QMutex mutexGo;
};
