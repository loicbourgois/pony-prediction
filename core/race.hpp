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
    bool addPony(const Pony & pony);
    bool isValid(){return valid;}
private:
    std::vector<Pony> ponies;
    std::vector<int> top5;
    QString name;
    bool valid;
};
