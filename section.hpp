#ifndef SECTION_HPP
#define SECTION_HPP

#include <QVector>
#include <cmath>

class Section
{
public:
    Section();
    Section(double lenght, double qgr, double angle);
     ~Section();
     void SetLenght(double lenght);
     double GetLenght() const;
     void SetQgr(double Qgr);
     double GetQgr() const;
     void SetAngle(double angle);
     double GetAngle() const;
     double GetSinus() const;
     double GetTangens() const;
     int GetCounter() const;
     int GetNumber() const;

 private:
    double iangle;
    double iqgr;
    double ilenght;
    int number;
    static int icounter;
};

#endif //SECTION_HPP
