#pragma once

#include <random>
#include <QDebug>
#include <QVector>
#include <QString>

class Util
{
public:
    static float getRandomFloat(const float & min, const float & max);
    static int getRandomInt(const int & min, const int & max);
    template<typename T> static void debug(const T & arg);
    static void debug(const QVector<int> & arg);
};
