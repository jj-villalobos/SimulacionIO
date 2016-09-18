#include "Prob.h"

Prob::Prob(){}

Prob::~Prob(){}

int Prob::unif(int range_from, int range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    
    return distr(generator);
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

