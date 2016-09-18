#ifndef COLAP_H
#define COLAP_H
#include <vector>
#include <utility>

using namespace std;

//typedef pair<double,int> evento;

struct Event {
    Event (double t, int i) {
        time = t;
        id = i;
    }
    double time; int id ;
};

class colaP {
    public:
        colaP();
        virtual ~colaP();
        void encolar(Event*);
        Event desencolar();
        bool vacio();
        void elimCambio();
        void vaciar();
        vector<Event*> heap;
    protected:
    private:

};

#endif // COLAP_H
