#include "layer.hpp"
#include <QString>
#include <QDebug>

Layer::Layer() :
  neurons(),
  inputsPerNeuron(),
  outputs()
{

}

Layer::Layer(const int & neuronCount, const int & inputsPerNeuron) :
  Layer()
{
  this->inputsPerNeuron = inputsPerNeuron;
  for(int i = 0 ; i < neuronCount ; i++)
  {
    neurons.push_back(Neuron());
  }
}

Layer::~Layer()
{

}

/*void Layer::compute(const QVector<float> & inputs)
{
  outputs.clear();
  QVector< QVector<float> > vectorInputs;
  for(int i = 0 ; i < inputs.size() / inputsPerNeuron ; i++)
  {
    vectorInputs.push_back(QVector<float>());
    for(int j = i*inputsPerNeuron ; j < (i+1)*inputsPerNeuron ; j++)
    {
      vectorInputs[i].push_back(inputs[j]);
    }
  }
  for(int i = 0 ; i < vectorInputs.size() ; i++)
  {
    neurons[0].compute(vectorInputs[i]);
    outputs.push_back(neurons[0].getOutput());
  }
}*/

void Layer::compute(const QVector<float> & inputs)
{
  outputs.clear();
  if(inputsPerNeuron)
  {
    QVector< QVector<float> > vectorInputs;
    for(int i = 0 ; i < inputs.size() / inputsPerNeuron ; i++)
    {
      vectorInputs.push_back(QVector<float>());
      for(int j = i*inputsPerNeuron ; j < (i+1)*inputsPerNeuron ; j++)
      {
        vectorInputs[i].push_back(inputs[j]);
      }
    }
    for(int i = 0 ; i < neurons.size() && i < vectorInputs.size() ; i++)
    {
      neurons[i].compute(vectorInputs[i]);
    }
  }
  else
  {
    for(int i = 0 ; i < neurons.size() ; i++)
    {
      neurons[i].compute(inputs);
    }
  }
  for(int i = 0 ; i < neurons.size() ; i++)
  {
    outputs.push_back(neurons[i].getOutput());
  }
}

void Layer::mutateRandomly()
{
  for(int i = 0 ; i < neurons.size() ; i++)
    neurons[i].mutateRandomly();
}

void Layer::mutate(const float & mutationFrequency,
                   const float & mutationIntensity)
{
  for(int i = 0 ; i < neurons.size() ; i++)
    neurons[i].mutate(mutationFrequency, mutationIntensity);
}
