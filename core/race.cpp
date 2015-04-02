#include "race.hpp"

Race::Race() :
    ponies(),
    orderOnArrival(),
    name("no-race")
{

}

Race::Race(const QString & name) :
    ponies(),
    orderOnArrival(),
    name(name)
{

}

Race::~Race()
{

}

void Race::addPony(const Pony & pony)
{
}
