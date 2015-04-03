#pragma once

#include <vector>

class Neuron
{
public:
    Neuron();
    ~Neuron();
private:
    bool compute(const std::vector<float> &inputs);
    bool addWeight();
    std::vector<float> weights;
    float output;
};
