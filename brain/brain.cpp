#include "brain.hpp"
#include <QDebug>
#include "core/util.hpp"

int Brain::idCount = 0;
float Brain::mutationRatio = 0.5f;
int Brain::ratiosToSaveCount = 100;
float Brain::bestRatio = -1.0f;
Brain Brain::bestBrain;
int Brain::bestBrainId = -1;
QVector<float> Brain::lastNratios;
QMutex Brain::mutexBestRatio;
QMutex Brain::mutexBestBrain;
QMutex Brain::mutexBestBrainId;
QMutex Brain::mutexLastNratios;

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

void Brain::evaluate1()
{
  mutexBestRatio.lock();
  bool isBest = (ratio > bestRatio);
  mutexBestRatio.unlock();
  if(isBest)
  {
    mutexBestRatio.lock();
    bestRatio = ratio;
    qDebug() << "######################################################";
    qDebug() << "# Brain" << id << ":" << ratio;
    qDebug() << "######################################################";
    mutexBestRatio.unlock();
    copyToBestBrain();
  }
  mutateFromBest();
  reset();
}

void Brain::evaluate2()
{
  // Evaluate if best brain or not
  mutexBestRatio.lock();
  bool hasBestRatio = (ratio > bestRatio);
  mutexBestRatio.unlock();
  if(hasBestRatio)
  {
    mutexBestBrainId.lock();
    bestBrainId = id;
    qDebug() << "######################################################";
    qDebug() << "# Brain" << id << ":" << ratio;
    qDebug() << "######################################################";
    mutexBestBrainId.unlock();
    copyToBestBrain();
  }
  // If is best brain
  mutexBestBrainId.lock();
  bool isBest = (id == bestBrainId);
  mutexBestBrainId.unlock();
  if(isBest)
  {
    mutexBestRatio.lock();
    bestRatio = ratio;
    mutexBestRatio.unlock();
  }
  // Calculate average ratio
  float averageRatio = 0.0f;
  mutexLastNratios.lock();
  lastNratios.push_back(ratio);
  while(lastNratios.size() > ratiosToSaveCount)
    lastNratios.pop_front();
  for(int i = 0 ; i<lastNratios.size() ; i++)
  {
    averageRatio += lastNratios[i];
  }
  averageRatio /= (float)lastNratios.size();
  mutexLastNratios.unlock();
  // If worse than average : mutate
  mutexBestRatio.lock();
  bool doMutate = (ratio <= averageRatio);
  mutexBestRatio.unlock();
  if(doMutate)
  {
    mutexBestBrainId.lock();
    if(bestBrainId == id)
    {
      mutexBestRatio.lock();
      bestRatio = 0;
      mutexBestRatio.unlock();
    }
    mutexBestBrainId.unlock();
    mutateFromBest();
    reset();
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

