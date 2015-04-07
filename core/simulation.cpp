#include "simulation.hpp"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

int Simulation::idBestBrain = 0;
const QString Simulation::DATABASE_NAME = "pony_prediction";

Simulation::Simulation() :
  races(),
  brainThreads(),
  inputs(),
  wantedResults()
{
}

Simulation::~Simulation()
{

}

bool Simulation::loadRaces(const QDate & startingDay, const QDate & endingDay)
{
  bool ok = true;
  QString error = "";
  int totalRacesCount = 0;
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName(DATABASE_NAME);
  db.setUserName("root");
  db.setPassword("");
  //
  if (ok && !db.open())
  {
    ok = false;
    error = "Database error occurred";
  }
  if(ok)
  {
    for(QDate date = startingDay ; date <= endingDay ; date = date.addDays(1))
    {
      QSqlQuery query;
      query.prepare(" SELECT coursesCheval, victoiresCheval, placesCheval, "
                    " coursesJockey, victoiresJockey, placesJockey, "
                    " prix, arrivee, partant "
                    " FROM day" + date.toString("yyyyMMdd") +
                    " ORDER BY prix, arrivee ");
      if(!query.exec())
        qDebug() << "Error with query for" << "day"+date.toString("yyyyMMdd")<< ":" << query.lastError().text();
      else
      {
        QSqlRecord record = query.record();
        //
        int courseChevalCol = record.indexOf("coursesCheval");
        int victoiresChevalCol = record.indexOf("victoiresCheval");
        int placesChevalCol = record.indexOf("placesCheval");
        int courseJockeyCol = record.indexOf("coursesJockey");
        int victoiresJockeyCol = record.indexOf("victoiresJockey");
        int placesJockeyCol = record.indexOf("placesJockey");
        int raceNameColumn = record.indexOf("prix");
        int orderOnArricalColumn = record.indexOf("arrivee");
        int partantsColumn = record.indexOf("partant");
        //
        float coursesCheval = 0.0f;
        float victoiresCheval = 0.0f;
        float placesCheval = 0.0f;
        float coursesJockey = 0.0f;
        float victoiresJockey = 0.0f;
        float placesJockey = 0.0f;
        QString lastRaceName = "no-race";
        QString raceName = "";
        QString orderOnArrival = "";
        QString lastOrderOnArrival = "";
        int partants = 0;
        //
        Race race;
        while (query.next())
        {
          // New race
          raceName = query.value(raceNameColumn).toString();
          orderOnArrival = query.value(orderOnArricalColumn).toString();
          if(raceName != lastRaceName
             || orderOnArrival != lastOrderOnArrival)
          {
            lastRaceName = raceName;
            lastOrderOnArrival = orderOnArrival;
            partants = query.value(partantsColumn).toInt();
            if(race.isValid())
              races.push_back(race);
            race = Race(date, raceName, orderOnArrival, partants);
            totalRacesCount++;
          }
          // New pony
          coursesCheval = query.value(courseChevalCol).toFloat();
          victoiresCheval = query.value(victoiresChevalCol).toFloat();
          placesCheval = query.value(placesChevalCol).toFloat();
          coursesJockey = query.value(courseJockeyCol).toFloat();
          victoiresJockey = query.value(victoiresJockeyCol).toFloat();
          placesJockey = query.value(placesJockeyCol).toFloat();
          race.addPony(coursesCheval, victoiresCheval, placesCheval,
                       coursesJockey, victoiresJockey, placesJockey);
        }
      }
    }
    qDebug() << races.size() << "/" << totalRacesCount << "races are ok";
  }
  if(!ok)
  {
    qDebug() << error;
  }

  return ok;
}

void Simulation::prepareData()
{
  for(int i = 0 ; i < races.size() ; i++)
  {
    races[i].prepareData();
    inputs.push_back(races[i].getInputs());
    wantedResults.push_back(races[i].getResult());
  }
}

void Simulation::loadBrains(const int &count,
                            const int &layersPerBrain,
                            const int &neuronsPerLayer)
{
  for(int i = 0 ; i < count ; i++)
  {
    brainThreads.push_back(
          new BrainThread(layersPerBrain,
                          neuronsPerLayer,
                          (int)Simulation::INPUTS_PER_NEURON_FIRST_LAYER,
                          inputs,
                          wantedResults,
                          qrand()));
  }
}

void Simulation::start()
{
  for(int i = 0 ; i < brainThreads.size() ; i++)
  {
    brainThreads[i]->start();
  }
}
