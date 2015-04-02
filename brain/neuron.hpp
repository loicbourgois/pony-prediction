#pragma once

#include <vector>

class Neuron
{
public:
    Neuron();
    ~Neuron();
private:
    std::vector<float> weights;
    float output;
};
