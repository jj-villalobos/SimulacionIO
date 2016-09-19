





#include <unistd.h> // sleep
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include "Prob.h"
#include "ColaP.h"
#include "Est.h"
//#include "ContenedorEventos.cpp"
//#include <math.h>
//#define Infinito 9999
#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif
using namespace std;
/*
#if _WIN32
#include <windows.h>
#endif // _WIN32
*/

        enum Eventos {
            e_llega_msg_a_C1 = 1               ,
            e_llega_pkg_a_C1                   ,
            e_termina_de_atender_msg_en_C1_S1  ,
            e_termina_de_atender_pkg_en_C1_S2  ,
            e_llega_ACK_a_C1                   ,
            e_llega_msg_malo_a_C1              ,
            e_llega_msg_a_C2                   ,
            e_termina_de_atender_msg_en_C2     ,
            e_llega_pkg_a_C3                   ,
            e_fin_procesa_pkg_en_C3            ,
            e_se_activa_el_timer
        };
        
        static double Reloj = 0.0; //Reloj global de la simulacion

struct Pkg {
    public:
    Pkg () { tiempoArribo = Reloj; }
    int id;

    private:
        double tiempoArribo; //hora del Reloj en la que llega el archivo
};

struct Msg {
    Msg () { tiempoArribo = Reloj; }

    private:
        double tiempoArribo; //hora del Reloj en la que llega el archivo
};

struct ACK {
    int id;          //numero de Pkgs
    double tiempoArribo; //hora del Reloj en la que llega el archivo
};

struct Computadora {
    bool ocupada;
};
      