#include "brain.hpp"
#include <QDebug>
#include "core/util.hpp"

int Brain::idCount = 0;
float Brain::bestRatio = -1.0f;
float Brain::mutationRatio = 0.5f;
QMutex Brain::mutexBestRatio;
QMutex Brain::mutexBestBrain;
Brain Brain::bestBrain;

Brain::Brain() :
  layers(),
  outputs(),
  result(),
  score(0),
  attempts(0),
  ratio(0),
  id()
{
  id = idCount;
  idCount++;
}

Brain::Brain(const int &layerCount, const int &neuronsPerLayer,
             const int & inputsPerNeuronFirstLayer) :
  Brain()
{
  layers.push_back(Layer(neuronsPerLayer, inputsPerNeuronFirstLayer));
  for(int i = 1 ; i < layerCount ; i++)
  {
    layers.push_back(Layer(neuronsPerLayer));
  }
}

Brain::~Brain()
{
}

void Brain::compute(const QVector<float> & inputs)
{
  layers[0].compute(inputs);
  for(int i = 1 ; i < layers.size() ; i++)
  {
    layers[i].compute(layers[i-1].getOutputs());
  }
  outputs = layers[layers.size()-1].getOutputs();
  attempts++;
}

void Brain::learn(const Result &wantedResult)
{
  if(wantedResult.getFirst() == result.getFirst())
  {
    score += 1;
  }
  else
  {
  }
  ratio = (float)score /  (float)attempts;
}

void Brain::prepareResult(const int & ponyCount)
{
  QVector<int> top5;
  QVector<float> arrivee;
  for(int i = 0 ; i < ponyCount ; i++)
  {
    arrivee.push_back(outputs[i]);
  }
  for(int i = 0 ; i<5 ; i++)
  {
    int bestId = 0;
    int bestRatio = 0.0f;
    for(int j = 0 ; j<arrivee.size(); j++)
    {
      if(arrivee[j] > bestRatio)
      {
        bestId = j;
        bestRatio = arrivee[j];
      }
    }
    top5.push_back(bestId);
    arrivee.remove(bestId);
  }
  result = Result(top5);
}

void Brain::prepareNewRun()
{
  evaluate();
  mutateFromBest();
  reset();
}

void Brain::mutateRandomly()
{
  for(int i = 0 ; i < layers.size() ; i++)
    layers[i].mutateRandomly();
}

void Brain::mutateFromBest()
{
  copyFromBestBrain();
  for(int i = 0 ; i < layers.size() ; i++)
    layers[i].mutate(mutationRatio);
}

void Brain::evaluate()
{
  bool copy = false;
  mutexBestRatio.lock();
  if(ratio > bestRatio)
  {
    copy = true;
    bestRatio = ratio;
    qDebug() << "######################################################";
    qDebug() << "# Brain" << id << ":" << ratio;
    qDebug() << "######################################################";
  }
  mutexBestRatio.unlock();
  if(copy)
  {
    copyToBestBrain();
  }
}

void Brain::reset()
{
  outputs.clear();
  result = Result();
  score = 0;
  attempts = 0;
  ratio = 0;
}

void Brain::copyToBestBrain()
{
  mutexBestBrain.lock();
  bestBrain.layers = layers;
  bestBrain.outputs = outputs;
  bestBrain.result = result;
  bestBrain.score = score;
  bestBrain.attempts = attempts;
  bestBrain.ratio = ratio;
  mutexBestBrain.unlock();
}

void Brain::copyFromBestBrain()
{
  mutexBestBrain.lock();
  layers = bestBrain.layers;
  outputs = bestBrain.outputs;
  result = bestBrain.result;
  score = bestBrain.score;
  attempts = bestBrain.attempts;
  ratio = bestBrain.ratio;
  mutexBestBrain.unlock();
}

