#include "Est.h"


Est::Est()
{
    //ctor
}

Est::~Est()
{
    //dtor
}

double Est::promedioCola(list <int> * cola)
{
   typename list <int> :: iterator it;

    double prom = 0;

    for(it = cola->begin(); it != cola->end(); ++it)
    {
         prom += *it;
    }
    prom = prom / cola->size();
    return prom;
}

double Est::promedioPermanencia(list <double> * cola)
{
   typename list <double> :: iterator it;

    double prom = 0;

    for(it = cola->begin(); it != cola->end(); ++it)
    {
         prom += *it;
    }
    prom = prom / cola->size();
    return prom;
}


double Est::promedioToken(list <int>* comp1, list <int>*comp2, list <int>*comp3)
{
    typename list <int> :: iterator it;
    double prom = 0;

    for(it = comp1->begin(); it != comp1->end(); ++it)
    {
         prom += *it;
    }
    for(it = comp2->begin(); it != comp2->end(); ++it)
    {
         prom += *it;
    }
    for(it = comp3->begin(); it != comp3->end(); ++it)
    {
         prom += *it;
    }
    prom = prom / (comp1->size() + comp2->size() + comp3->size());
    return prom;

}
