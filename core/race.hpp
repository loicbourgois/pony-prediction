#pragma once

#include <QVector>
#include "pony.hpp"
#include <QString>

class Race
{
public:
    Race();
    Race(const QString & name, const QString & top5);
    ~Race();
    bool addPony(const float &coursesCheval, const float &victoiresCheval, const float &placesCheval);
    bool isValid(){return valid;}
    void prepareData();
    const QVector<float> & getData() const {return data;}
private:
    QVector<Pony> ponies;
    QVector<int> top5;
    QString name;
    bool valid;
    QVector<float> data;
};
