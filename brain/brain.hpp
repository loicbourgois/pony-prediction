#pragma once

#include "layer.hpp"
#include "core/result.hpp"

class Brain
{
public:
    Brain();
    Brain(const int &layerCount, const int &neuronsPerLayer, const int & inputsPerNeuronFirstLayer = -1);
    ~Brain();
    void compute(const QVector<float> &inputs);
    void learn(const Result & wantedResult);
    void prepareResult(const int & ponyCount);

    const float & getRatio() const {return (float)score /  (float)attempts;}
private:
    QVector<Layer> layers;
    QVector<float> outputs;
    Result result;
    /*int age;
    int wins;
    int loses;
    float ratio;*/
    int score;
    int attempts;
};
