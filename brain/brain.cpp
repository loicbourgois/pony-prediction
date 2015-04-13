#include "brain.hpp"
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include "core/util.hpp"

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
  inputs(),
  outputs(),
  neuronBlueprints(),
  neurons(),
  result(),
  score(0.0f),
  attempts(0),
  ratio(0.0f),
  neuronsCount(neuronsPerLayer * layersCount)
{
  id = idCount;
  idCount++;
  initBlueprints();
  initNeurons();
}

Brain::Brain(const QString & path)
{
  id = idCount;
  idCount++;
  load(path);
  initNeurons();
}

Brain::~Brain()
{
  idCount--;
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

void Brain::save(const QString & path)
{
  bool ok = true;
  QString error = "";
  QFile file(path);
  if (ok && !file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    ok = false;
    error = "Can't open";
  }
  if(ok)
  {
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("brain");
    {
      xml.writeTextElement("ratio", QString::number(ratio));
      xml.writeTextElement("neurons-count",
                           QString::number(neuronBlueprints.size()));
      xml.writeTextElement("inputs-count",
                           QString::number(inputs.size()));
      xml.writeTextElement("outputs-count",
                           QString::number(outputs.size()));
      xml.writeTextElement("weights-count",
                           QString::number(weights.size()));
      QString weightsStr = QString::number(weights[0], 'f', 6);
      for(int i = 1 ; i < weights.size() ;  i++)
        weightsStr += ";" + QString::number(weights[i], 'f', 6);
      xml.writeTextElement("weights", weightsStr);
      xml.writeStartElement("neurons");
      {
        for(int i = 0 ; i < neuronBlueprints.size() ;  i++)
        {
          neuronBlueprints[i].writeToXML(xml);
        }
      }
      xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
  }
}

void Brain::load(const QString & path)
{
  // Counts
  inputsCount = 0;
  weightsCount = 0;
  neuronsCount = 0;
  outputsCount = 0;
  QString weightsStr;
  QStringList weightsStrList;
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
  QXmlStreamReader xml(&file);
  while (!xml.atEnd())
  {
    QXmlStreamReader::TokenType token = xml.readNext();
    if(token == QXmlStreamReader::StartElement)
    {
      if(xml.name() == "neurons-count")
      {
        neuronsCount = xml.readElementText().toInt();
        neuronBlueprints.clear();
        neurons = QVector<Neuron>(neuronsCount, Neuron());
      }
      if(xml.name() == "inputs-count")
      {
        inputsCount = xml.readElementText().toInt();
        inputs = QVector<float>(inputsCount, 0.0f);
      }
      if(xml.name() == "outputs-count")
      {
        outputsCount = xml.readElementText().toInt();
        outputs = QVector<float>(outputsCount, 0.0f);
      }
      if(xml.name() == "weights-count")
      {
        weightsCount = xml.readElementText().toInt();
        weights = QVector<float>(weightsCount, 0.0f);
      }
      if(xml.name() == "weights")
      {
        weightsStr = xml.readElementText();
        weightsStrList = weightsStr.split(';');
        for(int i = 0 ; i < weightsStrList.size() ; i++)
          weights[i] = weightsStrList[i].toFloat();
      }
      if(xml.name() == "neuron")
      {
        neuronBlueprints.push_back(NeuronBlueprint(xml));
      }
    }
  }
}

/*******************************************************************************
* Inits
*******************************************************************************/

void Brain::initBlueprints()
{
  //
  inputs = QVector<float>(inputsCount, 0.0f);
  int w = inputsCount / neuronsPerLayer
      + neuronsPerLayer * neuronsPerLayer * (layersCount-1);
  weights.clear();
  for(int i = 0 ; i < w ; i++)
    weights.push_back(Util::getRandomFloat(0.0f, 1.0f));
  outputs = QVector<float>(outputsCount, 0.0f);
  //
  neuronBlueprints.clear();
  for(int i = 0 ; i < neuronsPerLayer * layersCount ; i++)
    neuronBlueprints.push_back(NeuronBlueprint());
  // External inputs
  int inPerNeur = inputsCount / neuronsPerLayer;
  for(int id = 0 ; id < neuronsPerLayer ; id++)
  {
    for(int j = 0 ; j < inPerNeur ; j++)
    {
      int id2 = id*inPerNeur + j;
      neuronBlueprints[id].addExternalInputId(id2);
    }
  }
  // Neuronal inputs
  for(int i = 1 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i*neuronsPerLayer + j;
      for(int k = 0 ; k < neuronsPerLayer ; k++)
      {
        int id2 = (i-1)*neuronsPerLayer + k;
        neuronBlueprints[id].addNeuronalInputId(id2);
      }
    }
  }
  // Weights : first layer : same for all
  for(int id = 0 ; id < neuronsPerLayer ; id++)
  {
    for(int id2 = 0 ; id2 < inPerNeur ; id2++)
    {
      neuronBlueprints[id].addWeightId(id2);
    }
  }
  // Weights : others layer : unique for all
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
        neuronBlueprints[id].addWeightId(id2);
      }
    }
  }
}

void Brain::initBlueprints2()
{
  neuronsPerLayer = 20;
  layersCount = 4;
  inputsCount = 4*20;
  int inputsPerNeuron = 4;
  neuronsCount = neuronsPerLayer * layersCount;
  weightsCount = 5 * layersCount - 1;
  //
  inputs = QVector<float>(inputsCount, 0.0f);
  weights.clear();
  for(int i = 0 ; i < weightsCount ; i++)
    weights.push_back(Util::getRandomFloat(0.0f, 1.0f));
  outputs = QVector<float>(outputsCount, 0.0f);
  //
  neuronBlueprints.clear();
  for(int i = 0 ; i < neuronsCount ; i++)
    neuronBlueprints.push_back(NeuronBlueprint());
  // External inputs
  for(int i = 0 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i * neuronsPerLayer + j;
      for(int k = 0 ; k < inputsPerNeuron ; k++)
      {
        int id2 = j * inputsPerNeuron + k;
        neuronBlueprints[id].addExternalInputId(id2);
      }
    }
  }
  // Neuronal inputs
  for(int i = 1 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i*neuronsPerLayer + j;
      neuronBlueprints[id].addNeuronalInputId(id - neuronsPerLayer);
    }
  }
  // Weights : first layer : same for all
  for(int i = 0 ; i < neuronsPerLayer ; i++)
  {
    int id = i;
    for(int j = 0 ; j < inputsPerNeuron ; j++)
    {
      int id2 = j;
      neuronBlueprints[id].addWeightId(id2);
    }
  }
  for(int i = 1 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i*neuronsPerLayer + j;
      for(int k = 0 ; k < inputsPerNeuron+1 ; k++)
      {
        int id2 = i * (inputsPerNeuron+1) + k -1;
        neuronBlueprints[id].addWeightId(id2);
      }
    }
  }
}

void Brain::initNeurons()
{
  //
  neurons.clear();
  for(int i = 0 ; i < neuronsCount ; i++)
  {
    neurons.push_back(Neuron());
  }
  //
  for(int i = 0 ; i < neurons.size() ; i++)
  {
    NeuronBlueprint blueprint = neuronBlueprints[i];
    for(int j = 0 ;
        j < blueprint.externalInputIds.size() ;
        j++)
    {
      float * a = &inputs[blueprint.externalInputIds[j]];
      neurons[i].addExternalInput(a);
    }
    for(int j = 0 ;
        j < blueprint.neuronalInputIds.size() ;
        j++)
    {
      float * a = neurons[blueprint.neuronalInputIds[j]].getOutputAdress();
      neurons[i].addNeuronalInput(a);
    }
    for(int j = 0 ;
        j < blueprint.weightIds.size() ;
        j++)
    {
      float * a = &weights[blueprint.weightIds[j]];
      neurons[i].addWeight(a);
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
  bestBrain.neuronBlueprints = neuronBlueprints;
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
  neuronBlueprints = bestBrain.neuronBlueprints;
  result = Result();
  score = 0;
  attempts = 0;
  ratio = 0;
  mutexBestBrain.unlock();
  initNeurons();
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
