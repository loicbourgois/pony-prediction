#pragma once

#include <QVector>

class Result
{
  public:
    Result();
    Result(const QVector<int> &orderOnarrival);
    ~Result();
    const int & get(const int & i) const {return orderOnarrival[i];}
  private:
    QVector<int> orderOnarrival;
};
