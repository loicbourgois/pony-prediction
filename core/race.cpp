#include "race.hpp"

#include <QStringList>
#include <QDebug>

Race::Race() :
    ponies(),
    top5(),
    name("no-race")
{

}

Race::Race(const QString & name, const QString & top5) :
    ponies(),
    top5(),
    name(name)
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
        qDebug() << error;
    }
}

Race::~Race()
{

}

void Race::addPony(const Pony & pony)
{
    ponies.push_back(pony);
}
