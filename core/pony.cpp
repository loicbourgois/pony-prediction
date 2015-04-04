#include "pony.hpp"

Pony::Pony() :
  ratioWin(0.0f),
  ratioTop5(0.0f)
{

}

Pony::Pony(const float & races, const float & wins, const float & inTop5) :
  ratioWin(wins / races),
  ratioTop5(inTop5 / races)
{

}

Pony::~Pony()
{
}
