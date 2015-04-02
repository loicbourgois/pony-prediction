#include "brain.hpp"

Brain::Brain() :
    layers(),
    age(0),
    wins(0),
    loses(0),
    ratio(0.0f)
{

}

Brain::Brain(const int &layerCount, const int &neuronsPerLayer) :
    Brain()
{
    for(int i = 0 ; i < layerCount ; i++)
    {
        layers.push_back(Layer(neuronsPerLayer));
    }
}

Brain::~Brain()
{

}

