#pragma once

#include <QVector>

class Neuron
{
  public:
    Neuron();
    ~Neuron();
    void compute(const QVector<float> &inputs);
    void mutateRandomly();
    const float & getOutput() const{return output;}
  private:
    void addWeight();
    QVector<float> weights;
    float output;
};
