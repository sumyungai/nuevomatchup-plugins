//doubles
#include <map>
#include <cmath>
#include <iostream>
#include "mydouble.h"
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdint>
using namespace std;


double Mydouble::count = 0;
std::map <double, long int> Mydouble::doubles;

double Mydouble::r = 1e-21; // default behavior
int Mydouble::digits = 21;
long int Mydouble::man = 0; //default
long int Mydouble::exp = 0;
void Mydouble::set_resolution(int digits)
{
    Mydouble::r = pow(10.0, -1*digits);
    Mydouble::digits = digits;
}

const std::map<double, long int>& Mydouble::getmap()
{
    return Mydouble::doubles;
}
int Mydouble::getr()
{
    return Mydouble::digits;
}
void Mydouble::map_insert(double x)
{
    if (doubles.find(x) == doubles.end()) {
        doubles.insert(std::pair<double, long int>(x, 0));
    }
    doubles[x]++;
    count++;
}
double Mydouble::fixed_x(double x1)
{
    return round(x1/r)*r;
}

const long int Mydouble::getman()
{
    return Mydouble::man;
}

const long int Mydouble::getexp()
{
    return Mydouble::exp;
}

void Mydouble::setexp(long int e)
{
    Mydouble::exp = e;
}

void Mydouble::setman(long int e)
{
    Mydouble::man = e;
}



double Mydouble::quantize(double x1)
{
    long int exponent = getexp();
    long int mantissa = getman();
    if (Mydouble::getr() < 21)
    {
        return fixed_x(x1);
    }
    else if (mantissa == 0 && exponent == 0)
    {
        return x1;
    }
    else 
    {
    struct Quan *p = (struct Quan*)&x1;
    
    
    p->man &= mantissa;
    
    if (p->exp> 1023 + exponent)
    {
      p->exp = 1023 + exponent;
      //cout<<p->exp<<"\n";
    }
    else if (p->exp< 1023 - exponent)
    {
      p->exp = 1023 - exponent;

    }
    
    }
    
    return x1;


}



Mydouble::Mydouble(double x1) 
{
    this->x = Mydouble::quantize(x1);
}

Mydouble::Mydouble(const Mydouble &&other) : x(other.x)
{ }

Mydouble::Mydouble(const Mydouble &other) : x(other.x)
{
    map_insert(x);
}

int Mydouble::getcount()
{
    return count;
}

Mydouble operator- (const Mydouble &d1, const Mydouble &d2)
{
    Mydouble sub(Mydouble::quantize(d1.x) - Mydouble::quantize(d2.x));
    Mydouble::quantize(sub);
    return sub;
}

ostream& operator<< (ostream& out, const Mydouble &d1)
{
    out << d1.x;
    return out;
}


Mydouble operator* (const Mydouble &d1, const Mydouble &d2)
{
    Mydouble mult(Mydouble::quantize(d1.x) * Mydouble::quantize(d2.x));
    Mydouble::quantize(mult);
    return mult;
}

Mydouble operator/ (const Mydouble &d1, const Mydouble &d2)
{
    const Mydouble div(Mydouble::quantize(d1.x) / Mydouble::quantize(d2.x));
    Mydouble::quantize(div);
    return div;
}

bool operator> (const Mydouble &d1, const Mydouble &d2)
{
    return (d1.x>d2.x);
}

double Mydouble::getdouble() 
{
    return x;
}

bool operator< (const Mydouble &d1, const Mydouble &d2)
{
    return d1.x < d2.x;
}

Mydouble floor(const Mydouble &d1)
{
    Mydouble d2(floor(Mydouble::quantize(d1.x)));
    return d2;
}

Mydouble abs(const Mydouble &d1)
{
    Mydouble d2(abs(Mydouble::quantize(d1.x)));
    return d2;
}
