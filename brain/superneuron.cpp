#include "superneuron.hpp"

SuperNeuron::SuperNeuron()
{

}

SuperNeuron::~SuperNeuron()
{

}

void SuperNeuron::addExternalInput(float * input)
{
  externalInputs.push_back(input);
}

void SuperNeuron::addNeuronalInput(float * input)
{
  neuronalInputs.push_back(input);
}

void SuperNeuron::addWeight(float * weight)
{
  weights.push_back(weight);
}
