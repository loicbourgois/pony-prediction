#pragma once

#include "layer.hpp"
#include "core/result.hpp"

class Brain
{
  public:
    Brain();
    Brain(const int &layerCount, const int &neuronsPerLayer,
          const int & inputsPerNeuronFirstLayer);
    ~Brain();
    void compute(const QVector<float> &inputs);
    void learn(const Result & wantedResult);
    void prepareResult(const int & ponyCount);
    void mutateRandomly();
    void resetScore() {score = attempts = 0;}
    const float & getRatio() const {return ratio;}
    const int & getId() const {return id;}
  private:
    static int idCount;
    QVector<Layer> layers;
    QVector<float> outputs;
    Result result;
    int score;
    int attempts;
    float ratio;
    int id;
};
