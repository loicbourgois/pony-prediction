#pragma once

#include <QDate>
#include "race.hpp"
#include <vector>

class Simulation
{
public:
    Simulation();
    ~Simulation();
    bool loadData(const QDate &startingDay, const QDate &endingDay);
private:
    std::vector<Race> races;
};
