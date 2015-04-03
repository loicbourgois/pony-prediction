#include "brain.hpp"
#include <QDebug>

Brain::Brain() :
    layers(),
    age(0),
    wins(0),
    loses(0),
    ratio(0.0f)
{

}

Brain::Brain(const int &layerCount, const int &neuronsPerLayer, const int & inputsPerNeuronFirstLayer) :
    Brain()
{
    layers.push_back(Layer(neuronsPerLayer, inputsPerNeuronFirstLayer));
    for(int i = 1 ; i < layerCount ; i++)
    {
        layers.push_back(Layer(neuronsPerLayer));
    }
}

Brain::~Brain()
{

}

void Brain::compute(const QVector<float> & inputs)
{
    layers[0].compute(inputs);
    for(int i = 1 ; i < layers.size() ; i++)
    {
        layers[i].compute(layers[i-1].getOutputs());
    }
}

void Brain::teach()
{

}

