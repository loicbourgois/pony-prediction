#pragma once

#include <QVector>

class Result
{
  public:
    Result();
    Result(const QVector<int> &top5);
    ~Result();
    //const int & getFirst() const {return top5[0];}
    const int & get(const int & i) const {return top5[i];}
    const QVector<int> & getTop5() const {return top5;}
  private:
    QVector<int> top5;
};
