#include "simulation.hpp"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

Simulation::Simulation()
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

bool Simulation::loadData(const QDate & startingDay, const QDate & endingDay)
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
            while (query.next())
            {
                // New race ?
                raceName = query.value(raceNameColumn).toString();
                if(raceName != lastRaceName)
                {
                    lastRaceName = raceName;
                    top5 = query.value(top5Column).toString();
                    races.push_back(Race(raceName, top5));
                }
                // New pony
                coursesCheval = query.value(courseChevalCol).toFloat();
                victoiresCheval = query.value(victoiresChevalCol).toFloat();
                placesCheval = query.value(placesChevalCol).toFloat();
                races[races.size()-1].addPony(Pony(coursesCheval, victoiresCheval, placesCheval));
            }
        }
        qDebug() << races.size();
    }
    if(!ok)
    {
        qDebug() << error;
    }
    return ok;
}
