#pragma once

#include <QDate>
#include "race.hpp"
#include "brain/brain.hpp"
#include <QThread>
#include "brain/brain-thread.hpp"

class Simulation
{
  public:
    Simulation();
    ~Simulation();
    bool loadRaces(const QDate &startingDay, const QDate &endingDay);
    void prepareData();
    void loadBrains(const int & count, const int & layersPerBrain,
                    const int & neuronsPerLayer);
    void start();
    static const int INPUTS_PER_NEURON_FIRST_LAYER = 2;
  private:
    QVector<Race> races;
    QVector<Brain> brains;
    QVector<BrainThread*> brainThreads;
    QVector< QVector<float> > inputs;
    QVector<Result> wantedResults;
};
