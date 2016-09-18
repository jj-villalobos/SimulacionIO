#include <math.h>
#include <stdlib.h>
#include <random>
#define pi 3.1415926535
#ifndef PROB_H
#define PROB_H


class Prob
{
    public:
        Prob();
        virtual ~Prob();
        static int unif(int, int);
        static double norm(double, double);
        static double disB();
        static double exp(double);

    protected:
    private:
};

#endif // PROB_H
