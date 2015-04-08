#include "race.hpp"
#include <QStringList>
#include <QDebug>

Race::Race() :
  ponies(),
  orderOnArrival(),
  name("no-race"),
  valid(false),
  data(),
  result()
{
}

Race::Race(const QDate & date, const QString & name, const QString & orderOnArrivalStr, const int & partants) :
  ponies(),
  orderOnArrival(),
  name(name),
  valid(true),
  data(),
  date(date),
  partants(partants)
{
  bool ok = true;
  QString error = "";
  QStringList orderOnArrivalStrList = orderOnArrivalStr.split('-');
  if(ok && orderOnArrivalStrList.size()!=5)
  {
    ok = false;
    error = "Le top 5 ne contient pas 5 chevaux : '" + orderOnArrivalStr + "'";
  }
  if(ok)
  {
    for(int i = 0 ; i < orderOnArrivalStrList.size() ; i++)
    {
      orderOnArrival.push_back(orderOnArrivalStrList[i].toInt());
    }
  }
  if(!ok)
  {
    valid = false;
    //qDebug() << error;
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

bool Race::isValid()
{
  bool ok = true;
  QString error = "";
  if(valid)
  {
    if(ok && ponies.size() < 5)
    {
      ok = false;
      error = "Il y a moins de 5 chevaux";
    }
    if(ok && ponies.size() > 20)
    {
      ok = false;
      error = "Il y a plus de 20 chevaux dans la course";
    }
    /* if(ok && partants != ponies.size())
    {
      ok = false;
      error = "Problème avec le nombre de cheveaux : " + QString::number(partants) + " instead of " + QString::number(ponies.size());
    }*/
    if(!ok)
    {
      valid = false;
      //qDebug() << error << date << name ;
    }
  }
  return valid;
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
  result = Result(orderOnArrival);
}
