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

void Layer::compute(const QVector<float> & inputs)
{
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
  outputs.clear();
  for(int i = 0 ; i < neurons.size() ; i++)
  {
    outputs.push_back(neurons[i].getOutput());
  }
}

void Layer::mutateRandomly()
{
  for(int i = 0 ; i < neurons.size() ; i++)
  {
    neurons[i].mutateRandomly();
  }
}
