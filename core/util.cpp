#include "util.hpp"

float Util::getRandomFloat(const float &min, const float &max)
{
    return ((static_cast <float>(rand())) * (max - min) / (static_cast<float>(RAND_MAX))) + min;
}
int Util::getRandomInt(const int &min, const int &max)
{
    int r = getRandomFloat(min-1, max+1);
    if(r < min || r > max)
        r = getRandomInt(min, max);
    return r;
}
template<typename T> void Util::debug(const T &arg)
{
    qDebug() << arg;
}
void Util::debug(const QVector<int> & arg)
{
    QString str = " | ";
    for(int i = 0; i < arg.size() ; i++)
        str += QString::number(arg[i]) + " | ";
    qDebug() << str;
}
