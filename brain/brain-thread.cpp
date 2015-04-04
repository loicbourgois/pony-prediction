#include "brain-thread.hpp"
#include <QDebug>
#include <QTime>

BrainThread::BrainThread(const QVector<QVector<float> > & inputs,
                         const QVector<Result> & wantedResults,
                         const int & seed) :
  QThread(),
  go(false),
  inputs(inputs),
  wantedResults(wantedResults),
  brain(),
  seed(seed)
{

}

BrainThread::BrainThread(const int &layerCount, const int &neuronsPerLayer,
                         const int & inputsPerNeuronFirstLayer,
                         const QVector<QVector<float> > & inputs,
                         const QVector<Result> & wantedResults,
                         const int & seed) :
  BrainThread(inputs, wantedResults, seed)
{
  brain = Brain(layerCount, neuronsPerLayer, inputsPerNeuronFirstLayer);
}

BrainThread::~BrainThread()
{

}

void BrainThread::run()
{
  qsrand(seed);
  go = true;
  //while(go)
  {
    for(int i = 0 ; i < inputs.size() ; i++)
    {
      brain.compute(inputs[i]);
      brain.prepareResult(inputs[i].size() / 2);
      brain.learn(wantedResults[i]);
    }
    qDebug()<<"Brain" << (int)QThread::currentThreadId() << brain.getRatio();
  }
}
