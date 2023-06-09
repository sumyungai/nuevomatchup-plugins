#ifndef MYDOUBLE_H
#define MYDOUBLE_H
#include <map>
#include <cmath>
#include <iostream>
#include <float.h>
#include <cstdio>
#include <cstdint>
using namespace std;

struct Quan
{
    uint64_t man : 52;
    uint64_t exp : 11;
    uint64_t sign : 1;
};


class Mydouble
{
public:  
        Mydouble(double x = 0.0);
        Mydouble(const Mydouble &other); 
        Mydouble(const Mydouble &&other);
        static double quantize(double x1);
        static int getr();
        static const std::map<double, long int>& getmap();
        static int getcount();
        static double fixed_x(double x1);
        double getdouble();
        static const long int getman();
        static const long int getexp();
        static void setexp(long int exp);
        static void setman(long int man);
        std::string convert();
        static double r;
        static int digits; 
        static long int exp;
        static long int man;

private:
        static std::map <double,long int> doubles;
        static double count;
        double x;
        double quantize_x(double x);
        void map_insert(double num);

public:

    static void set_resolution(int digits);

    friend Mydouble operator- (const Mydouble &d1, const Mydouble &d2);
    friend std::ostream& operator<< (std::ostream &out, const Mydouble &d1);
    friend bool operator==(const Mydouble &d1, const Mydouble &d2);
    friend Mydouble operator* (const Mydouble &d1, const Mydouble &d2);
    friend Mydouble operator/ (const Mydouble &d1, const Mydouble &d2);
    friend bool operator> (const Mydouble &d1, const Mydouble &d2);
    friend bool operator>= (const Mydouble &d1, const Mydouble &d2);
    friend bool operator< (const Mydouble &d1, const Mydouble &d2);
    friend Mydouble floor(const Mydouble &d1);
    friend Mydouble abs(const Mydouble &d1);

    template <typename T>
    friend Mydouble operator- (const T &d1, const Mydouble &d2)
    {
        Mydouble temp = d1 - quantize(d2.x);
        quantize(temp.x);
        return temp;
    }
    template <typename T>
    friend Mydouble operator+ (const T &d1, const Mydouble &d2)
    {
        Mydouble temp = d1 + quantize(d2.x);
        quantize(temp.x);
        return temp;
    }

    template <typename T>
    friend Mydouble operator/ (const T &d1, const Mydouble &d2)
    {
        Mydouble temp = d1 / quantize(d2.x);
        quantize(temp.x);
        return temp;
    }

    template <typename T>
    friend Mydouble operator* (const T &d1, const Mydouble &d2)
    {
        Mydouble temp = d1 * quantize(d2.x);
        quantize(temp.x);
        return temp;
    }

    template <typename T>
    Mydouble operator- (const T &d2) const
    {
        Mydouble temp = quantize(this->x) - d2;
        quantize(temp.x);
        return temp;
    }

    template <typename T>
    Mydouble operator+ (const T &d2) const
    {
        Mydouble temp = quantize(this->x)+d2;
        quantize(temp.x);
        return temp;
    }
    template <typename T>
    Mydouble operator/ (const T &d2) const
    {
        Mydouble temp = quantize(this->x) / d2;
        quantize(temp.x);
        return temp;
    }

    template <typename T>
    Mydouble operator* (const T &d2) const
    {
        Mydouble temp = quantize(this->x) * d2;
        quantize(temp.x);
        return temp;
    }
    
    template <typename T>
    bool operator==(const T &d2) const
    {
        bool temp = quantize(this->x) == d2;
        return temp;
    }
    friend bool operator==(const Mydouble &d1, const Mydouble &d2)
    {
        return d1.x==d2.x;
    }
    template <typename T>
    friend bool operator==(const T &d1, const Mydouble &d2)
    {
        bool temp = d1 == quantize(d2.x);
        return temp;
    }

    template <typename T>
    Mydouble& operator=(T &d2) 
    {
        this->x = d2;
        quantize(this->x);
        return (*this);
    }

    Mydouble& operator=(const Mydouble &d)
    {
        if (&d == this) { return *this; }
        this->x = quantize(d.x);
        quantize(this->x);
        return (*this);
    }

    template <typename T>
    friend bool operator> (const T &d1, const Mydouble &d2)
    {
        bool temp = d1> quantize(d2.x); 
        return temp;
    }

    template <typename T>
    bool operator> (const T &d2) const
    {
        bool temp = this->x > d2;
        return temp;
    }

    template <typename T>
    friend bool operator>= (const T &d1, const Mydouble &d2)
    {   
        bool temp = d1>= d2.x;
        return temp;
    }

    template <typename T>
    bool operator>= (const T &d2) const
    {
        bool temp = quantize(this->x)>=d2;
        return temp;
    }
    
    bool operator>= (const Mydouble &d2) const
    {
        bool temp = this->x>=d2.x;
        return temp;
    } 

    template <typename T>
    friend T& operator-= (T &d1,const Mydouble &d2)
    {
        d1= d1-quantize(d2.x);
        return d1;
    }
    template <typename T>
    Mydouble& operator-= (const T &d2) 
    {
        this->x = quantize(this->x)-d2;
        quantize(this->x);
        return *this;
    }

    Mydouble& operator-= (const Mydouble &d2) 
    {
        this->x -= quantize(d2.x);
        quantize(this->x);
        return *this;
    }

    template <typename T>
    friend T& operator+= (T &d1,const Mydouble &d2)
    {
        d1= d1+quantize(d2.x);
        return d1;
    }

    template <typename T>
    Mydouble& operator+= (const T &d2) 
    {
        this->x = quantize(this->x)+d2;
        quantize(this->x);
        return *this;
    }
     
    Mydouble& operator+= (const Mydouble &d2) 
    {
        this->x += quantize(d2.x);
        quantize(this->x);
        return *this;
    }

template <typename T>
    friend T& operator*= (T &d1,const Mydouble &d2)
    {
        d1= d1*quantize(d2.x);
        return d1;
    }

    template <typename T>
    Mydouble& operator*= (const T &d2) 
    {
        this->x = quantize(this->x)*d2;
        quantize(*this);
        return *this;
    }
     
    Mydouble& operator*= (const Mydouble &d2) 
    {
        this->x *= quantize(d2.x);
        quantize(this->x);
        return *this;
    }



    template <typename T>
    friend bool operator< (const T &d1, const Mydouble &d2)
    {
        bool temp = d1<d2.x;
        return temp;
    }
    template <typename T>
    bool operator< (const T &d2) const
    {
        bool temp = this->x<d2;
        return temp;
    }
    
    template <typename T>
    bool operator!=(const T &d2) const
    {
        bool temp = !(this->x == d2);
        return temp;
    }

    template <typename T>
    friend bool operator!=(const T &d1, const Mydouble &d2)
    {
        bool temp = !(d1 == d2.x);
        return temp;
    }

    operator int() const
    {
        return (int)x;
    }

    operator size_t() const
    {
        return (size_t)x;
    }

    operator double() const
    {
        return x;
    }
    operator float() const
    {
        return (float)x;
    }
    operator unsigned() const
    {
        return (unsigned)x;
    }

    void set_max() { x = DBL_MAX; }
    void set_min() { x = DBL_MIN; }
    void set_epsilon() { x = DBL_EPSILON; }
    void set_next()
    {
        if (x < DBL_MAX) {
            x = nextafter(x, DBL_MAX);
        }
    }
    void set_prev()
    {
        if (x > DBL_MIN) {
            x = nextafter(x, DBL_MIN);
        }
    }

};

 
#endif

