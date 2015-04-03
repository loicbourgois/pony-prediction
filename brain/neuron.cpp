#include "neuron.hpp"
#include <QDebug>
#include <math.h>

Neuron::Neuron() :
    weights(),
    output(0.0f)
{

}

Neuron::~Neuron()
{

}

bool Neuron::compute(const std::vector<float> & inputs)
{
    float absoluteWeight = 0.0f;
    output = 0.0f;
    for(int i = 0 ; i < inputs.size() ; i++)
    {
        if(weights.size() <= i )
            addWeight();
        if(inputs[i] > 1 || inputs[i] < -1)
            qDebug() << "Inputs value out of range [-1;1] : " << inputs[i];
        output += inputs[i] * weights[i];
        absoluteWeight += fabs(weights[i]);
    }
    output /= absoluteWeight;
    if(output > 1 || output < -1)
        qDebug() << "Output value out of range [-1;1] : " << output;
}

bool Neuron::addWeight()
{
    weights.push_back(0.0f); // 0 or random ?
}
