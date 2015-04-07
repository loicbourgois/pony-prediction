#include "result.hpp"

Result::Result() :
  orderOnarrival()
{

}

Result::Result(const QVector<int> & orderOnarrival)
{
  this->orderOnarrival.clear();
  for(int i = 0 ; i < orderOnarrival.size() ; i++)
  {
    this->orderOnarrival.push_back(orderOnarrival[i]);
  }
}

Result::~Result()
{

}
