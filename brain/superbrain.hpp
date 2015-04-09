#pragma once

#include <QVector>
#include <brain/superneuron.hpp>

class SuperBrain
{
  public:
    SuperBrain(const int & inputsCount,
               const int & neuronsPerLayer,
               const int & layersCount);
    ~SuperBrain();
  private:
  void init();
    void initNeurons();
    void initExternalInputs();
    void initNeuronalInputs();
    void initWeights();

    int inputsCount;
    int neuronsPerLayer;
    int layersCount;

    QVector<float> weights;
    QVector<float> outputs;
    QVector<SuperNeuron> neurons;
    QVector<float> inputs;
};
