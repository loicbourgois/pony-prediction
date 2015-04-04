#include "neuron.hpp"
#include <QDebug>
#include <math.h>
#include "core/util.hpp"

Neuron::Neuron() :
  weights(),
  output(0.0f)
{

}

Neuron::~Neuron()
{

}

void Neuron::compute(const QVector<float> & inputs)
{
  float absoluteWeight = 0.0f;
  output = 0.0f;
  for(int i = 0 ; i < inputs.size() ; i++)
  {
    if(weights.size() <= i )
      addWeight();
    if(!(inputs[i] <= 1 && inputs[i] >= -1))
      qDebug() << "Inputs value out of range [-1;1] : " << inputs[i];
    if(!(weights[i] <= 1 && weights[i] >= -1))
      qDebug() << "Inputs value out of range [-1;1] : " << inputs[i];
    output += inputs[i] * weights[i];
    absoluteWeight += fabs(weights[i]);
  }
  output /= absoluteWeight;
  if(!(output <= 1 && output >= -1))
    qDebug() << "Output value out of range [-1;1] : " << output;
}

void Neuron::mutateRandomly()
{
  for(int i = 0 ; i < weights.size() ; i++)
  {
    weights[i] = Util::getRandomFloat(-1.0f, 1.0f);
  }
}

void Neuron::addWeight()
{
  weights.push_back(Util::getRandomFloat(-1.0f, 1.0f));
}
