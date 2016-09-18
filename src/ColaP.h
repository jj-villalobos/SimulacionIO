#ifndef COLAP_H
#define COLAP_H
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

struct Event {
    Event (double t, int i) {
        time = t;
        id = i;
    }
    double time; int id ;
};

class colaP {
    public:
        virtual 
        ~colaP();
        colaP();
        bool vacio();
        void encolar(Event*);
        void elimCambio();
        void vaciar();
        Event desencolar();
        vector<Event*> heap;
    protected:
    private:
};

#endif // COLAP_H
