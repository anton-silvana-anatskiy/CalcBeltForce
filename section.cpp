#include "section.hpp"

Section::Section()
{
    ilenght = 1;
    iqgr = 1;
    iangle = 1;
    icounter++;
    number = icounter;
}

Section::Section(double lenght, double qgr, double angle)
{
    ilenght = lenght;
    iqgr = qgr;
    iangle = angle;
    icounter++;
    number = icounter;
}

Section::~Section(){}

int Section::icounter = 0;

double Section::GetLenght() const
{
       return ilenght;
}

void Section::SetLenght(double lenght)
{
    ilenght = lenght;
}

double Section::GetQgr() const
{
    double x = iqgr/(3.6*2);
    return x;
}

void Section::SetQgr(double Qgr)
{
    iqgr = Qgr;
}

double Section::GetAngle() const
{
    return iangle;
}

double Section::GetSinus() const
{
    double PI = 3.14;
    double radians = (PI * iangle) / 180;
    return sin(radians);
}

double Section::GetTangens() const
{
    double PI = 3.14;
    double radians = (PI * iangle) / 180;
    return tan(radians);
}

void Section::SetAngle(double angle)
{
    iangle = angle;
}

int Section::GetCounter() const
{
    return icounter;
}

int Section::GetNumber() const
{
    return number;
}
