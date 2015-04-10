#include "brain.hpp"
#include "core/util.hpp"

#include <QDebug>

int Brain::idCount = 0;
int Brain::ratiosToSaveCount = 100;
Brain Brain::bestBrain(1,1,1,1);
float Brain::averageRatio = 0.0f;
QVector<float> Brain::lastNratios = QVector<float>(100, 0.0f);
float Brain::mutationFrequency = 0.0f;
float Brain::mutationIntensity = 0.0f;
QMutex Brain::mutexBestBrain;
QMutex Brain::mutexAverageRatio;
QMutex Brain::mutexLastNratios;
QMutex Brain::mutexMutationFrequency;
QMutex Brain::mutexMutationIntensity;

Brain::Brain(const int & inputsCount,
             const int & neuronsPerLayer,
             const int & layersCount,
             const int & outputsCount) :
  id(-1),
  inputsCount(inputsCount),
  neuronsPerLayer(neuronsPerLayer),
  layersCount(layersCount),
  outputsCount(outputsCount),
  weights(),
  outputs(),
  neurons(),
  inputs(),
  result(),
  score(0.0f),
  attempts(0),
  ratio(0.0f)
{
  initRandom();
  initNeurons();
  initExternalInputs();
  initNeuronalInputs();
  initWeights();


  saveToBestBrain();
}

Brain::~Brain()
{

}

void Brain::compute(const QVector<float> & inputs)
{
  for(int i = 0 ; i < this->inputs.size() ; i++)
    this->inputs[i] = 0.0f;
  for(int i = 0 ; i < inputs.size() && i < this->inputs.size() ; i++)
    this->inputs[i] = inputs[i];
  for(int i = 0 ; i < neurons.size() ; i++)
    neurons[i].compute();
  for(int i = 0 ; i < outputsCount ; i++)
  {
    int i2 = (neurons.size() - outputs.size()) + i;
    outputs[i] = neurons[i2].getOutput();
  }
}

void Brain::prepareResult(const int & ponyCount)
{
  QVector<int> arrivalOrder;
  QVector<float> arrivalDisorder;
  for(int i = 0 ; i < ponyCount ; i++)
    arrivalDisorder.push_back(outputs[i]);
  int tmp = arrivalDisorder.size();
  for(int i = 0 ; i<tmp ; i++)
  {
    int bestId = 0;
    int bestRatio = 0.0f;
    for(int j = 0 ; j<arrivalDisorder.size(); j++)
    {
      if(arrivalDisorder[j] > bestRatio)
      {
        bestId = j;
        bestRatio = arrivalDisorder[j];
      }
    }
    arrivalOrder.push_back(bestId);
    arrivalDisorder.remove(bestId);
  }
  result = Result(arrivalOrder);
}

void Brain::learn(const Result & wantedResult)
{
  attempts++;
  if(result.get(0) == wantedResult.get(0))
    score += 1.0f;
  ratio = score /  (float)attempts;
}

void Brain::evaluate()
{
  // Calculate average ratio
  addRatioToAverage(ratio);
  // Evaluate if best brain or not
  mutexAverageRatio.lock();
  float averagetmp = averageRatio;
  mutexAverageRatio.unlock();
  if(ratio > averagetmp)
  {
    Util::addLog("Brain " + QString::number(id)
                 + " : " + QString::number(ratio, 'f', 6)
                 + " : " + QString::number(averagetmp, 'f', 6));
    saveToBestBrain();
  }
  mutateFromBest();
}

void Brain::mutateFromBest()
{
  loadFromBestBrain();
  mutexMutationFrequency.lock();
  float mutationFrequency = Brain::mutationFrequency;
  mutexMutationFrequency.unlock();
  mutexMutationIntensity.lock();
  float mutationIntensity = Brain::mutationIntensity;
  mutexMutationIntensity.unlock();
  for(int i = 0 ; i < weights.size() ; i++)
  {
    if(Util::getRandomFloat(0.0f, 1.0f) < mutationFrequency)
    {
      if(Util::getRandomFloat(-1.0f, 1.0f) > 0)
        weights[i] += mutationIntensity;
      else
        weights[i] -= mutationIntensity;
      if(weights[i] > 1.0f)
        weights[i] = 1.0f;
      if(weights[i] < -1.0f)
        weights[i] = -1.0f;
    }
  }
}

/*******************************************************************************
* Inits
*******************************************************************************/

void Brain::initRandom()
{
  id = idCount;
  idCount++;
  inputs = QVector<float>(inputsCount, 0.0f);
  int w = inputsCount / neuronsPerLayer
      + neuronsPerLayer * neuronsPerLayer * (layersCount-1);
  weights.clear();
  for(int i = 0 ; i < w ; i++)
    weights.push_back(Util::getRandomFloat(0.0f, 1.0f));
  outputs = QVector<float>(outputsCount, 0.0f);
}

void Brain::initNeurons()
{
  neurons.clear();
  for(int i = 0 ; i < neuronsPerLayer * layersCount ; i++)
    neurons.push_back(Neuron());
}

void Brain::initExternalInputs()
{
  int inPerNeur = inputsCount / neuronsPerLayer;
  for(int i = 0 ; i < neuronsPerLayer ; i++)
  {
    for(int j = 0 ; j < inPerNeur ; j++)
    {
      neurons[i].addExternalInput(&inputs[i*inPerNeur + j]);
    }
  }
}

void Brain::initNeuronalInputs()
{
  for(int i = 1 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i*neuronsPerLayer + j;
      for(int k = 0 ; k < neuronsPerLayer ; k++)
      {
        int id2 = (i-1)*neuronsPerLayer + k;
        neurons[id].addNeuronalInput(neurons[id2].getOutputAdress());
      }
    }
  }
}

void Brain::initWeights()
{
  // First layer : same weights for all
  int inPerNeur = inputsCount / neuronsPerLayer;
  for(int i = 0 ; i < neuronsPerLayer ; i++)
  {
    for(int j = 0 ; j < inPerNeur ; j++)
    {
      neurons[i].addWeight(&weights[j]);
    }
  }
  // Other layers : unique weights
  for(int i = 1 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i*neuronsPerLayer + j;
      for(int k = 0 ; k < neuronsPerLayer ; k++)
      {
        int id2 = inPerNeur
            + (i-1)*neuronsPerLayer*neuronsPerLayer
            + j*neuronsPerLayer
            + k;
        neurons[id].addWeight(&weights[id2]);
      }
    }
  }
}

/*******************************************************************************
* Statics
*******************************************************************************/

void Brain::saveToBestBrain()
{
  mutexBestBrain.lock();
  bestBrain.id = 0;
  bestBrain.inputsCount = inputsCount;
  bestBrain.neuronsPerLayer = neuronsPerLayer;
  bestBrain.layersCount = layersCount;
  bestBrain.outputsCount = outputsCount;
  bestBrain.weights = weights;
  bestBrain.outputs = QVector<float>(outputsCount, 0.0f);
  bestBrain.neurons = QVector<Neuron>();
  bestBrain.inputs = QVector<float>(inputsCount, 0.0f);
  bestBrain.result = Result();
  bestBrain.score = score;
  bestBrain.attempts = attempts;
  bestBrain.ratio = ratio;
  mutexBestBrain.unlock();
}

void Brain::loadFromBestBrain()
{
  mutexBestBrain.lock();
  id = id;
  inputsCount = bestBrain.inputsCount;
  neuronsPerLayer = bestBrain.neuronsPerLayer;
  layersCount = bestBrain.layersCount;
  outputsCount = bestBrain.outputsCount;
  weights = bestBrain.weights;
  outputs = QVector<float>(outputsCount, 0.0f);
  neurons = QVector<Neuron>();
  inputs = QVector<float>(inputsCount, 0.0f);
  result = Result();
  score = 0;
  attempts = 0;
  ratio = 0;
  mutexBestBrain.unlock();
  initNeurons();
  initExternalInputs();
  initNeuronalInputs();
  initWeights();
}

void Brain::addRatioToAverage(const float & ratio)
{
  mutexLastNratios.lock();
  lastNratios.push_back(ratio);
  while(lastNratios.size() > ratiosToSaveCount)
    lastNratios.pop_front();
  mutexLastNratios.unlock();
  updateAverageRatio();
}

void Brain::updateAverageRatio()
{
  mutexLastNratios.lock();
  mutexAverageRatio.lock();
  averageRatio = 0.0f;
  for(int i = 0 ; i<lastNratios.size() ; i++)
  {
    averageRatio += lastNratios[i];
  }
  averageRatio /= (float)lastNratios.size();
  mutexLastNratios.unlock();
  mutexAverageRatio.unlock();
}
