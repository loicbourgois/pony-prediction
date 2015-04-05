#include "brain-thread.hpp"
#include <QDebug>
#include <QTime>
#include "core/simulation.hpp"

BrainThread::BrainThread(const int &layerCount, const int &neuronsPerLayer,
                         const int & inputsPerNeuronFirstLayer,
                         const QVector<QVector<float> > & inputs,
                         const QVector<Result> & wantedResults,
                         const int & seed) :
  QThread(),
  go(false),
  inputs(inputs),
  wantedResults(wantedResults),
  brain(layerCount, neuronsPerLayer, inputsPerNeuronFirstLayer),
  seed(seed)
{
}

BrainThread::~BrainThread()
{

}

void BrainThread::run()
{
  qsrand(seed);
  go = true;
  int steps = 0;
  int dataId = 0;
  int stepsPerRun = inputs.size();
  int runs = 0;
  while(go)
  {
    brain.compute(inputs[dataId]);
    brain.prepareResult(inputs[dataId].size() / Simulation::INPUTS_PER_NEURON_FIRST_LAYER); // pony count
    brain.learn(wantedResults[dataId]);
    if(!(steps % stepsPerRun) && steps != 0)
    {
      runs++;
      brain.evaluate1();
    }
    dataId++;
    dataId %= inputs.size();
    steps++;
  }
}
