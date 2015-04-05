#pragma once

class Pony
{
  public:
    Pony();
    Pony(const float & races, const float & wins, const float & inTop5,
         const float & racesJockey, const float & winsJockey,
         const float & inTop5Jockey);
    const float & getRatioWin() const {return ratioWin;}
    const float & getRatioWinJockey() const {return ratioWinJockey;}
    const float & getRatioTop5() const {return ratioTop5;}
    const float & getRatioTop5Jockey() const {return ratioTop5Jockey;}
    ~Pony();
  private:
    float ratioWin;
    float ratioTop5;
    float ratioWinJockey;
    float ratioTop5Jockey;
};
