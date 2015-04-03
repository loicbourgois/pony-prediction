#pragma once

#include "layer.hpp"
#include <vector>

class Brain
{
public:
    Brain();
    Brain(const int & layerCount, const int & neuronsPerLayer);
    ~Brain();
private:
    std::vector<Layer> layers;
    int age;
    int wins;
    int loses;
    float ratio;
};
