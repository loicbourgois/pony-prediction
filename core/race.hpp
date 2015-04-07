#pragma once

#include <QVector>
#include "pony.hpp"
#include <QString>
#include <QDate>
#include "result.hpp"

class Race
{
  public:
    Race();
    Race(const QDate & date, const QString & name, const QString & orderOnArrivalStr, const int & partants);
    ~Race();
    void addPony(const float &coursesCheval, const float &victoiresCheval,
                 const float &placesCheval, const float & coursesJockey,
                 const float & victoiresJockey, const float & placesJockey);
    bool isValid();
    void prepareData();
    const QVector<float> & getInputs() const {return data;}
    const Result & getResult() const {return result;}
  private:
    QVector<Pony> ponies;
    QVector<int> orderOnArrival;
    QString name;
    QDate date;
    bool valid;
    QVector<float> data;
    Result result;
    int partants;
};
