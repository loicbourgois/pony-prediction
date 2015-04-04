#pragma once

#include <QVector>
#include "pony.hpp"
#include <QString>
#include "result.hpp"

class Race
{
public:
    Race();
    Race(const QString & name, const QString & top5);
    ~Race();
    void addPony(const float &coursesCheval, const float &victoiresCheval, const float &placesCheval);
    bool isValid(){return valid;}
    void prepareData();
    const QVector<float> & getInputs() const {return data;}
    const Result & getResult() const {return result;}
private:
    QVector<Pony> ponies;
    QVector<int> top5;
    QString name;
    bool valid;
    QVector<float> data;
    Result result;
};
