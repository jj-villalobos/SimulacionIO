#include <list>

#ifndef EST_H
#define EST_H

using namespace std;

class Est
{
    public:
        Est();
        virtual ~Est();
        static double promedioCola(list<int>*);
        static double promedioPermanencia(list<double>*);
        static double promedioToken(list <int>*, list <int>*, list <int>*);
    protected:
    private:
};

#endif // EST_H
