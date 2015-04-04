#pragma once

class Pony
{
  public:
    Pony();
    Pony(const float & races, const float & wins, const float & inTop5);
    const float & getRatioWin() const {return ratioWin;}
    const float & getRatioTop5() const {return ratioTop5;}
    ~Pony();
  private:
    float ratioWin;
    float ratioTop5;
};
