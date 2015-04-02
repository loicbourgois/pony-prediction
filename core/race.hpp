#pragma once

#include <vector>
#include "pony.hpp"
#include <QString>

class Race
{
public:
    Race();
    Race(const QString & name, const QString & top5);
    ~Race();
    void addPony(const Pony & pony);
private:
    std::vector<Pony> ponies;
    std::vector<int> top5;
    QString name;
};
