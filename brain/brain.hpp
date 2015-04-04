#pragma once

#include <QMutex>
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
    void prepareNewRun();
    void mutateRandomly();
    void mutateFromBest();
    void evaluate();
    void reset();
    const float & getRatio() const {return ratio;}
    const int & getId() const {return id;}
  private:
    void copyToBestBrain();
    void copyFromBestBrain();
    static int idCount;
    static float bestRatio;
    static float mutationRatio;
    static QMutex mutexBestRatio;
    static QMutex mutexBestBrain;
    static Brain bestBrain;
    QVector<Layer> layers;
    QVector<float> outputs;
    Result result;
    int score;
    int attempts;
    float ratio;
    int id;
};
