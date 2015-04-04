#include "result.hpp"

Result::Result() :
  top5()
{

}

Result::Result(const QVector<int> & top5)
{
  this->top5.clear();
  for(int i = 0 ; i < top5.size() ; i++)
  {
    this->top5.push_back(top5[i]);
  }
}

Result::~Result()
{

}
