#include "race.hpp"
#include <QStringList>
#include <QDebug>

Race::Race() :
  ponies(),
  top5(),
  name("no-race"),
  valid(false),
  data(),
  result()
{

}

Race::Race(const QString & name, const QString & top5) :
  ponies(),
  top5(),
  name(name),
  valid(true),
  data()
{
  bool ok = true;
  QString error = "";
  QStringList top = top5.split('-');
  if(ok && top.size()!=5)
  {
    ok = false;
    error = "Le top 5 ne contient pas 5 chevaux : '" + top5 + "'";
  }
  if(ok)
  {
    for(int i = 0 ; i < top.size() ; i++)
    {
      this->top5.push_back(top[i].toInt());
    }
  }
  if(!ok)
  {
    valid = false;
    qDebug() << error;
  }
}

Race::~Race()
{

}

void Race::addPony(const float & coursesCheval, const float & victoiresCheval,
                   const float & placesCheval, const float & coursesJockey,
                   const float & victoiresJockey, const float & placesJockey)
{
  bool ok = true;
  QString error = "";
  if(ok && coursesCheval < 1)
  {
    ok = false;
    error = "Le cheval a participé dans moins de 1 course";
  }
  if(ok && coursesJockey < 1)
  {
    ok = false;
    error = "Le jockey a participé dans moins de 1 course";
  }
  if(ok)
    ponies.push_back(Pony(coursesCheval, victoiresCheval, placesCheval,
                          coursesJockey, victoiresJockey, placesJockey));
  if(!ok)
  {
    valid = false;
    //qDebug() << error;
  }
}

void Race::prepareData()
{
  data.clear();
  for(int i = 0 ; i < ponies.size() ; i++)
  {
    data.push_back(ponies[i].getRatioWin());
    data.push_back(ponies[i].getRatioTop5());
    data.push_back(ponies[i].getRatioWinJockey());
    data.push_back(ponies[i].getRatioTop5Jockey());
  }
  result = Result(top5);
}
