#pragma once

#include <vector>
#include "neuron.hpp"

class Layer
{
public:
    Layer();
    Layer(const int & neuronCount);
    ~Layer();
private:
    std::vector<Neuron> neurons;
};
