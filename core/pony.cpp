#include "pony.hpp"
#include <QDebug>

Pony::Pony() :
  ratioWin(0.0f),
  ratioTop5(0.0f)
{

}

Pony::Pony(const float & races, const float & wins, const float & inTop5,
           const float & racesJockey, const float & winsJockey,
           const float & inTop5Jockey) :
  ratioWin(wins / races),
  ratioTop5(inTop5 / races),
  ratioWinJockey(winsJockey / racesJockey),
  ratioTop5Jockey(inTop5Jockey / racesJockey)
{
  if(!(0.0f <= ratioWin && ratioWin <= 1.0f))
    ratioWin = 0.0f;
  if(!(0.0f <= ratioTop5 && ratioTop5 <= 1.0f))
    ratioTop5 = 0.0f;
  if(!(0.0f <= ratioWinJockey && ratioWinJockey <= 1.0f))
    ratioWinJockey = 0.0f;
  if(!(0.0f <= ratioTop5Jockey && ratioTop5Jockey <= 1.0f))
    ratioTop5Jockey = 0.0f;
}

Pony::~Pony()
{
}
