#include "Prob.h"

Prob::Prob(){}

Prob::~Prob(){}

int Prob::unif(int a, int b)
{
     double dbun = ((double) rand() / (RAND_MAX+1)) * (b-a+1) + a;
     int intun = (int) dbun;
     return intun;
}

double Prob:: norm(double med, double v)    //metodo directo o metodo Box-Muller
{
    double r1 = ((double) rand() / (RAND_MAX));
    double r2 = ((double) rand() / (RAND_MAX));
    double z1 = sqrt(-2*log(r1))*cos(2*pi*r2);
    double x1 = med + sqrt(v)*z1;
    return x1;
}

double Prob::exp(double med) {
    double lambda = 1/med;
    /// variable aleatoria de distribuci�n uniforme r = R ( 0 , 1 )
    double r = ((double) rand() / (RAND_MAX));

    double x = log(1-r)/lambda;
    x*= -1;

    return x;
}

double Prob::disB()
{   double r = ((double) rand() / (RAND_MAX));
    double x = sqrt(80*r + 64);
    return x;
}

