#include "superbrain.hpp"
#include "core/util.hpp"

SuperBrain::SuperBrain(const int & inputsCount,
                       const int & neuronsPerLayer,
                       const int & layersCount) :
  inputsCount(inputsCount),
  neuronsPerLayer(neuronsPerLayer),
  layersCount(layersCount)
{
  init();
  initNeurons();
  initExternalInputs();
  initNeuronalInputs();
  initWeights();

  /*for(int i = 0 ; i< neurons.size() ; i++)
  {
    qDebug() << &neurons[i].output <<neurons[i].neuronalInputs;
    }*/

}

SuperBrain::~SuperBrain()
{

}

void SuperBrain::init()
{
  for(int i = 0 ; i < inputsCount ; i++)
    inputs.push_back(Util::getRandomFloat(0.0f, 1.0f));
  int w = inputsCount / neuronsPerLayer
      + neuronsPerLayer * neuronsPerLayer * (layersCount-1);
  for(int i = 0 ; i < w ; i++)
    weights.push_back(Util::getRandomFloat(0.0f, 1.0f));
}

void SuperBrain::initNeurons()
{
  for(int i = 0 ; i < neuronsPerLayer * layersCount ; i++)
    neurons.push_back(SuperNeuron());
}

void SuperBrain::initExternalInputs()
{
  int inPerNeur = inputsCount / neuronsPerLayer;
  for(int i = 0 ; i < neuronsPerLayer ; i++)
  {
    for(int j = 0 ; j < inPerNeur ; j++)
    {
      neurons[i].addExternalInput(&inputs[i*inPerNeur + j]);
    }
  }
}

void SuperBrain::initNeuronalInputs()
{
  for(int i = 1 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i*neuronsPerLayer + j;
      for(int k = 0 ; k < neuronsPerLayer ; k++)
      {
        int id2 = (i-1)*neuronsPerLayer + k;
        neurons[id].addNeuronalInput(neurons[id2].getOutputAdress());
      }
    }
  }
}

void SuperBrain::initWeights()
{
  // First layer
  int inPerNeur = inputsCount / neuronsPerLayer;
  for(int i = 0 ; i < neuronsPerLayer ; i++)
  {
    for(int j = 0 ; j < inPerNeur ; j++)
    {
      neurons[i].addWeight(&weights[j]);
    }
  }
  //
  for(int i = 1 ; i < layersCount ; i++)
  {
    for(int j = 0 ; j < neuronsPerLayer ; j++)
    {
      int id = i*neuronsPerLayer + j;
      for(int k = 0 ; k < neuronsPerLayer ; k++)
      {
        int id2 = inPerNeur
            + (i-1)*neuronsPerLayer*neuronsPerLayer
            + j*neuronsPerLayer
            + k;
        neurons[id].addWeight(&weights[id2]);
      }
    }
  }
}
