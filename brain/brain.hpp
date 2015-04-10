#pragma once

#include <QVector>
#include <QMutex>
#include "brain/neuron.hpp"
#include "core/result.hpp"

class Brain
{
  public:

    Brain(const int & inputsCount,
          const int & neuronsPerLayer,
          const int & layersCount,
          const int & outputsCount);
    ~Brain();

    void compute(const QVector<float> & inputs);
    void prepareResult(const int & ponyCount);
    void learn(const Result & wantedResult);
    void evaluate();
    void mutateFromBest();

  private:

    void initRandom();
    void initNeurons();
    void initExternalInputs();
    void initNeuronalInputs();
    void initWeights();

    int id;

    int inputsCount;
    int neuronsPerLayer;
    int layersCount;
    int outputsCount;

    QVector<float> weights;
    QVector<float> outputs;
    QVector<Neuron> neurons;
    QVector<float> inputs;

    Result result;
    float score;
    int attempts;
    float ratio;

  public:

    static void setMutationFrequency(const float & ratio){
      mutexMutationFrequency.lock();
      mutationFrequency = ratio;
      mutexMutationFrequency.unlock();}
    static void setMutationIntensity(const float & ratio){
      mutexMutationIntensity.lock();
      mutationIntensity = ratio;
      mutexMutationIntensity.unlock();}

  private:

    void saveToBestBrain();
    void loadFromBestBrain();

    static void addRatioToAverage(const float & ratio);
    static void updateAverageRatio();
    static int idCount;
    static int ratiosToSaveCount;
    static Brain bestBrain;
    static float averageRatio;
    static QVector<float> lastNratios;
    static float mutationFrequency;
    static float mutationIntensity;
    static QMutex mutexBestBrain;
    static QMutex mutexAverageRatio;
    static QMutex mutexLastNratios;
    static QMutex mutexMutationFrequency;
    static QMutex mutexMutationIntensity;
};
