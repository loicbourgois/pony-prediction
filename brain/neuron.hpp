#pragma once

#include <QVector>

class Neuron
{
public:
    Neuron();
    ~Neuron();
    void compute(const QVector<float> &inputs);
    const float & getOutput() const{return output;}
private:
    void addWeight();
    QVector<float> weights;
    float output;
};
