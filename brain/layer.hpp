#pragma once

#include "neuron.hpp"
#include <QVector>

class Layer
{
  public:
    Layer();
    Layer(const int & neuronCount, const int &inputsPerNeuronFirstLayer = 0);
    ~Layer();
    void compute(const QVector<float> & inputs);
    void mutateRandomly();
    void mutate(const float & mutationRatio);
    const QVector<float> & getOutputs() const {return outputs;}
  private:
    QVector<Neuron> neurons;
    int inputsPerNeuron;
    QVector<float> outputs;
};
