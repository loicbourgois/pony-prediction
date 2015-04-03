#pragma once

#include "layer.hpp"

class Brain
{
public:
    Brain();
    Brain(const int &layerCount, const int &neuronsPerLayer, const int & inputsPerNeuronFirstLayer = -1);
    ~Brain();
    void compute(const QVector<float> &inputs);
    void teach();
private:
    QVector<Layer> layers;
    int age;
    int wins;
    int loses;
    float ratio;
};
