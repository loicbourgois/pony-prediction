#include "layer.hpp"

Layer::Layer() :
    neurons()
{

}

Layer::Layer(const int & neuronCount)
{
    for(int i = 0 ; i < neuronCount ; i++)
    {
        neurons.push_back(Neuron());
    }
}

Layer::~Layer()
{

}
