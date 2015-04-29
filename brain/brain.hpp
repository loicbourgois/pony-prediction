#pragma once

#include <QVector>
#include <QMutex>
#include "brain/neuron.hpp"
#include "brain/neuron-blueprint.hpp"
#include "core/result.hpp"

class Brain
{
  public:

    Brain(const int & inputsCount,
          const int & neuronsPerLayer,
          const int & layersCount,
          const int & outputsCount);
    Brain(const QString & path);
    ~Brain();

    void compute(const QVector<float> & inputs);
    void prepareResult(const int & ponyCount);
    void learn(const Result & wantedResult);
    void evaluate();
    void mutateFromBest();

  private:

    void save(const QString & path);
    void load(const QString & path);
    void initBlueprints();
    void initBlueprints2();
    void initNeurons();



    int id;

    int neuronsPerLayer;
    int layersCount;

    int inputsCount;
    int weightsCount;
    int neuronsCount;
    int outputsCount;

    QVector<float> weights;
    QVector<float> inputs;
    QVector<float> outputs;
    QVector<NeuronBlueprint> neuronBlueprints;
    QVector<Neuron> neurons;

    Result result;
    float score;
    int attempts;
    float ratio;

  public:

    static void uploadBestBrain(const QString & ip);
    static void setMutationFrequency(const float & ratio){
      mutexMutationFrequency.lock();
      mutationFrequency = ratio;
      mutexMutationFrequency.unlock();}
    static void setMutationIntensity(const float & ratio){
      mutexMutationIntensity.lock();
      mutationIntensity = ratio;
      mutexMutationIntensity.unlock();}
    static void saveBestBrain(const QString & path){
      mutexBestBrain.lock();
      bestBrain.save(path);
      mutexBestBrain.unlock();
    }

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
