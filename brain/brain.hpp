#pragma once

#include <QMutex>
#include "layer.hpp"
#include "core/result.hpp"

class Brain
{
  public:
    Brain();
    Brain(const int &layerCount,
          const int &neuronsPerLayer,
          const int & inputsPerNeuron);
    ~Brain();
    void compute(const QVector<float> &inputs);
    void learn(const Result & wantedResult);
    void prepareResult(const int & ponyCount);
    void mutateRandomly();
    void mutateFromBest();
    void evaluate1();
    void evaluate2();
    void reset();
    //
    const float & getRatio() const {return ratio;}
    const int & getId() const {return id;}
    static void setMutationFrequency(const float & ratio){
      mutexMutationFrequency.lock();
      mutationFrequency = ratio;
      mutexMutationFrequency.unlock();}
    static void setMutationIntensity(const float & ratio){
      mutexMutationIntensity.lock();
      mutationIntensity = ratio;
      mutexMutationIntensity.unlock();}
    //
  private:
    void copyToBestBrain();
    void copyFromBestBrain();
    static void addRatio(const float & ratio);
    static void updateAverageRatio();
    static int idCount;
    static float mutationRatio;
    static int ratiosToSaveCount;
    static float bestRatio;
    static Brain bestBrain;
    static int bestBrainId;
    static QVector<float> lastNratios;
    static float averageRatio;
    static QMutex mutexBestRatio;
    static QMutex mutexBestBrain;
    static QMutex mutexBestBrainId;
    static QMutex mutexLastNratios;
    static QMutex mutexAverageRatio;
    static float mutationFrequency;
    static QMutex mutexMutationFrequency;
    static float mutationIntensity;
    static QMutex mutexMutationIntensity;
    QVector<Layer> layers;
    QVector<float> outputs;
    Result result;
    int score;
    int attempts;
    float ratio;
    int id;
    int neuronsPerLayer;
    int inputsPerNeuron;

};
