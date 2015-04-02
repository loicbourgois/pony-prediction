#ifndef PONY_HPP
#define PONY_HPP


class Pony
{
public:
    Pony();
    Pony(const float & races, const float & wins, const float & inTop5);
    ~Pony();
private:
    float ratioWin;
    float ratioTop5;
};

#endif // PONY_HPP
