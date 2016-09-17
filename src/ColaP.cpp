#include "ColaP.h"
#include <vector>
#include <iostream>

using namespace std;


colaP::colaP()
{

}

colaP::~colaP()
{

}


void colaP::elimCambio(){   //elimina el evento "cambiar token" de la cola
    for(vector<Event>::iterator it = heap.begin(); it != heap.end(); ++it){
        if((*it).id == 1){  //si encuentra un evento "cambiar token"
            heap.erase(it);     //lo borra
            break;
        }
    }
}

bool colaP::vacio()     //retorna si la cola de eventos se encuentra vacía o no
{
    return heap.empty();
}

void colaP::encolar(Event even){   //inserta un evento en la cola de eventos, ordenada por prioridad (tiempo)
    bool insertado = false;     //control de inserción
    if(vacio())     //caso de cola vacía
    {
        heap.push_back(even);
        insertado = true;
    }
    else
    {
        for(vector<Event>::iterator it = heap.begin(); it != heap.end(); ++it){
            if((*it).time < even.time){   //cuando encuentre un evento con mayor tiempo, inserta el evento actual (así mantiene el orden)
                heap.insert(it, even);
                insertado = true;
                break;
            }
        }
    }
    if (insertado == false){    //si no ha logrado insertar el evento, va de ultimo
        heap.push_back(even);
    }
}

Event colaP::desencolar(){     //desencola el evento con tiempo menor
    Event X = heap.back();
    heap.pop_back();
    return X;
}

void colaP::vaciar()        //vacía la cola
{
    heap.clear();
}
