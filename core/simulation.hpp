#pragma once

#include <QDate>
#include "race.hpp"
#include <QThread>
#include "brain/brain-thread.hpp"

class Simulation
{
  public:
    static const int INPUTS_PER_NEURON_FIRST_LAYER = 4;
    Simulation();
    ~Simulation();
    bool loadRaces(const QDate &startingDay, const QDate &endingDay);
    void prepareData();
    void loadBrains(const int & count, const int & layersPerBrain,
                    const int & neuronsPerLayer);
    void loadBrains(const QString & path, const int &count);
    void play();
    void pause();
    static int idBestBrain;
  private:
    static const QString DATABASE_NAME;
    QVector<Race> races;
    QVector<BrainThread*> brainThreads;
    QVector< QVector<float> > inputs;
    QVector<Result> wantedResults;
};
