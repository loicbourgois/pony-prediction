#include "simulation.hpp"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

Simulation::Simulation() :
    races(),
    brains(),
    data()
{
    //bool ok = true;
    //QString error = "error""";
    /*if(ok && !this->loadData("20150101", "20150101"))
    {
        ok = false;
        error = "Couldn't load data from database";
    }

    if(!ok)
    {
        qDebug() << error;
    }
    if(ok)
    {
        qDebug() << "Succes loading data";
    }*/
}

Simulation::~Simulation()
{

}

bool Simulation::loadRaces(const QDate & startingDay, const QDate & endingDay)
{
    bool ok = true;
    QString error = "";
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("pony-prediction");
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
        //
        for(QDate date = startingDay ; date <= endingDay ; date = date.addDays(1))
        {
            QSqlQuery query;
            query.prepare(" SELECT coursesCheval, victoiresCheval, placesCheval, "
                          " prix, arrivee "
                          " FROM day" + date.toString("yyyyMMdd") +
                          " ORDER BY prix ");
            query.bindValue(":day", "day" + startingDay.toString("yyyyMMdd"));
            query.exec();
            QSqlRecord record = query.record();
            //
            int courseChevalCol = record.indexOf("coursesCheval");
            int victoiresChevalCol = record.indexOf("victoiresCheval");
            int placesChevalCol = record.indexOf("placesCheval");
            int raceNameColumn = record.indexOf("prix");
            int top5Column = record.indexOf("arrivee");
            //
            float coursesCheval = 0.0f;
            float victoiresCheval = 0.0f;
            float placesCheval = 0.0f;
            QString lastRaceName = "no-race";
            QString raceName = "";
            QString top5 = "";
            //
            Race race;
            while (query.next())
            {
                // New race ?
                raceName = query.value(raceNameColumn).toString();
                if(raceName != lastRaceName)
                {
                    lastRaceName = raceName;
                    top5 = query.value(top5Column).toString();
                    if(race.isValid())
                        races.push_back(race);
                    race = Race(raceName, top5);
                }
                // New pony
                coursesCheval = query.value(courseChevalCol).toFloat();
                victoiresCheval = query.value(victoiresChevalCol).toFloat();
                placesCheval = query.value(placesChevalCol).toFloat();
                race.addPony(coursesCheval, victoiresCheval, placesCheval);
            }
        }
        qDebug() << races.size() << "races are ok";
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
        data.push_back(races[i].getData());
    }
}

bool Simulation::loadBrains(const int &count, const int &layersPerBrain, const int &neuronsPerLayer)
{
    for(int i = 0 ; i < count ; i++)
    {
        brains.push_back(Brain(layersPerBrain, neuronsPerLayer, (int)Simulation::INPUTS_PER_NEURON_FIRST_LAYER));
    }
}

void Simulation::run()
{
    /*int a = 0;
    while(true)
    {*/
        // Compute
        for(int i = 0 ; i < brains.size() ; i++)
        {
            for(int j = 0 ; j < data.size() ; j++)
            {
                brains[i].compute(data[j]);
                brains[i].teach();
            }
        }
       /* qDebug() << (a+=40);
        //
    }*/
}
