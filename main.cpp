#include <unistd.h> // sleep
#include <iostream>
#include <time.h>
#include <list>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

/*
#if _WIN32
#include <windows.h>
#endif // _WIN32
*/

#include "Prob.h"
#include "ColaP.h"
#include "Est.h"
using namespace std;

enum Eventos {
    e_llega_msg_a_C1 = 1               ,
    e_llega_pkg_a_C1                   ,
    e_termina_de_atender_pkg_en_C1_S1  ,
    e_termina_de_atender_pkg_en_C1_S2  ,
    e_llega_ACK_a_C1                   ,
    e_llega_msg_malo_a_C1              ,
    e_llega_msg_a_C2                   ,
    e_termina_de_atender_msg_en_C2     ,
    e_llega_pkg_a_C3                   ,
    e_fin_procesa_pkg_en_C3            ,
    e_se_activa_el_timer
};


/// ESTRUCTURAS USADAS PARA MODELAR LA SOLUCION------------------------------

struct computadora {
    char id;                        //identificador para referenciar a la computadora
    int archivos = 0;               //numero archivos enviados en el turno presente
    bool ocupada = false;
};


struct paquete {
    int tamano;          //numero de paquetes
    double tiempoArribo; //hora del reloj en la que llega el archivo
};

struct mensaje {
    int tamano;          //numero de paquetes
    double tiempoArribo; //hora del reloj en la que llega el archivo
};

struct ACK {
    int id;          //numero de paquetes
    double tiempoArribo; //hora del reloj en la que llega el archivo
};


///-------------------------------------------------------------------------------------------------------
///VARIABLES GLOBALES-------------------------------------------------------------------------------------
colaP colaEventos; //cola de tipo colaP, implementado por una lista de pares: double, int. El double es un tiempo y el int un tipo de evento a ejecutar

/// archivo global que guarda temporalmente el archivo más recientemente enviado
// archivo * enviado;

double reloj = 0.0; //reloj global de la simulación

static computadora
 *A = new computadora, //struct representando cada una de las computadoras, son globales
 *B = new computadora,
 *C = new computadora;

void restEst() ;


///------------------------------------------------------------------------------------------------------
///------------------------------------------------------------------------------------------------------
///ESTADISTICAS------------------------------------------------------------------------------------------

///estructuras de datos usadas para recolectar estadísticas a desplegar durante y después de la simulación
list <int> longitudA;     //guarda la longitud de la cola de la computadora A (incluyendo ambas prioridades) en cada paso de la simulacion
list <int> longitudB;     //guarda la longitud de la cola de la computadora B (incluyendo ambas prioridades) en cada paso de la simulacion
list <int> longitudC;     //guarda la longitud de la cola de la computadora C (incluyendo ambas prioridades) en cada paso de la simulacion
list <int> longitudAV1;   //guarda la longitud de la cola 1 del antivirus en cada paso de la simulacion
list <int> longitudAV2;   //guarda la longitud de la cola 2 del antivirus en cada paso de la simulacion

///------------------------------------------------------------------------------------------------------
///METODOS AUXILIARES PARA LA IMPLEMENTACION DE LOS EVENTOS----------------------------------------------


/*

    /**
    metodo auxiliar a los mandarArchivo de todas las computadoras.
    Se encarga de escoger al próximo archivo
    a enviar. Toma como criterio principal escoger al archivo
    más grande que se pueda enviar en el presente
    turno del token, así como la prioridad
    * -- /
archivo* elegirArchivo(computadora* com) {
    list<archivo *>::iterator it;
    archivo* elegido = NULL;

    for(it = com->colaArchivos1.begin(); it != com->colaArchivos1.end(); ++it) {
                                                        //primero se recorre la lista de prioridad 1. Ambas estan ordenadas
                                                        //descendentemente en orden de tamaño del archivo, por lo que al
        double tiempoArc = (*it)->tamano * 0.5;         //encontrar un archivo que pueda ser enviado en el turno del token,
        if(com->tiempoTok - tiempoArc >= 0)             //este será el más grande posible que pueda ser enviado. Si ninguno
        {                                               //cabe en el turno del token entonces se pasa a la lista de prioridad 2
            elegido = *it;                              //y se itera sobre ella de la misma manera
            com->colaArchivos1.erase(it);
            break;
        }
    }
    //solo se busca en la cola de prioridad 2 si en la cola 1 no se logró
    if(elegido==NULL) {                                                                         //escoger un archivo exitosamente
        for(it = com->colaArchivos2.begin(); it != com->colaArchivos2.end(); ++it) {
            double tiempoArc = (*it)->tamano * 0.5;
            if(com->tiempoTok - tiempoArc > 0) {
                elegido= *it;
                com->colaArchivos2.erase(it);
                break;
            }
        }
    }

    return elegido;  //se devuelve el archivo escogido, dándose la posibilidad de que sea nulo, lo que significaría que: no había archivos en ninguna cola o
}                    //ningun archivo en las colas podía ser enviado en el presente turno del token

int calcularRevisiones(archivo * arc) //metodo auxiliar para calcular el numero de revisiones hechas sobre el archivo basándose en la cantidad de virus que tenga
{
    int rev = 0;
    if(arc->virus == 0) //si no tenía ni un virus entonces solo se revisó una vez
    {
        rev = 1;
    }
    else if ( arc ->virus == 1) //si tenía 1 entonces se dieron 2 revisiones
    {
        rev = 2;
    }
    else if (arc->virus == 2 || arc->virus == 3) // con 2 o 3 se dan 3 revisiones
    {
        rev = 3;
    }
    return rev;  //devuelve cantidad de revisiones para guardarlo para las estadísticas
}

void restEst()                    //como las structs usadas son globales y la simulación puede correrse varias veces entonces se requiere
{                                 //poder restaurar los valores originales de cada struct para evitar acumulación errónea de archivos entre corridas
    A->colaArchivos1.clear();
    A->colaArchivos2.clear();
    A->archivos = 0;

    B->colaArchivos1.clear();
    B->colaArchivos2.clear();
    B->archivos = 0;

    C->colaArchivos1.clear();
    C->colaArchivos2.clear();
    C->archivos = 0;

    while(!AV->cola1.empty())     //las colas stl no tienen un clear directo, por lo que hubo que iterar
        AV->cola1.pop();

     while(!AV->cola2.empty())
        AV->cola1.pop();

    R->linea1 = false;
    R->linea2 = false;
}
*/

///> nombres delos eventos
static string eventos [] = {
    "llega msg a C1",
    "llega pkg a C1",
    "termina de atender pkg en C1 S1",
    "termina de atender pkg en C1 S2",
    "llega ACK a C1",
    "llega msg malo a C1",
    "llega msg a C1",
    "termina de atender msg en C2",
    "llega pkg a C3",
    "fin procesa pkg en C3",
    "se activa el timer"
};

static string eventoProcesado(int i) {
    return eventos[i];
}


//-------------------------------------------------------------
///---------------------------EVENTOS--------------------------
//-------------------------------------------------------------

///Todos los eventos se programaron sin parámetros para que se pudieran llamar más fácilmente

void llega_msg_a_C1                   (); // E1
void llega_pkg_a_C1                   (); // E2
void termina_de_atender_pkg_en_C1_S1  (); // E3
void termina_de_atender_pkg_en_C1_S2  (); // E4
void llega_ACK_a_C1                   (); // E5
void llega_msg_malo_a_C1              (); // E6
void llega_msg_a_C1                   (); // E7
void termina_de_atender_msg_en_C2     (); // E8
void llega_pkg_a_C3                   (); // E9
void fin_procesa_pkg_en_C3            (); // E10
void se_activa_el_timer               (); // E11


//-------------------------------------------------------------
//-------------------------------------------------------------


/**
 * \brief   METODO PRINCIPAL ENCARGADO DE LLAMAR A CADA EVENTO
 recibe un int des-encolado de la cola de eventos
// para decidir cual evento ejecutar a continuación
 * \param
 * \return
 */
void ejecutarEvento(int i) {
    switch (i){
        case 1 : {llega_msg_a_C1                 (); break;}
        case 2 : {llega_pkg_a_C1                 (); break;}
        case 3 : {termina_de_atender_pkg_en_C1_S1(); break;}
        case 4 : {termina_de_atender_pkg_en_C1_S2(); break;}
        case 5 : {llega_ACK_a_C1                 (); break;}
        case 6 : {llega_msg_malo_a_C1            (); break;}
        case 7 : {llega_msg_a_C1                 (); break;}  /// ojo malo
        case 8 : {termina_de_atender_msg_en_C2   (); break;}
        case 9 : {llega_pkg_a_C3                 (); break;}
        case 10: {fin_procesa_pkg_en_C3          (); break;}
        case 11: {se_activa_el_timer             (); break;}
    }
}



static void limpiarConsola () {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


int main() {
    srand(time(NULL));
    ///> semilla, necesaria para las funciones de probabilidad

    int numSim,
        tSim  ; ///> tiempo ma´ximo que puede durar la simulacio´n

    cout << "Ingrese el numero de veces que quiere correr la simulacion.\n\n"; cin >> numSim;
    cout  << endl << "Ingrese la duracion maxima de \n la simulacion [en segundos]\n\n"; cin >> tSim;

    double tiempoSimulacion = (int) tSim;


    string  resp;
    cout << endl << "Desea ver simulacion en modo lento (si/no)? \n\n"  ; cin >> resp;

    bool lento = (resp == "si") ? true: false;

    double tpromcolaA = 0;  ///variables para guardar promedios totales
    double tpromcolaB = 0;
    double tpromcolaC = 0;
    double tpromcolaAV1 = 0;
    double tpromcolaAV2 = 0;
    double tpromPer = 0;
    double tpromEnv = 0;
    double tpromRev = 0;


    for(int i = 0; i < numSim; i++) {

        A->id = 'A';
        B->id = 'B';
        C->id = 'C';

        /** la simulacioin inicia con los eventos
            1 y 2,  llega msg a c1 y
                    llega pkg a c1
        */

        ///             tiempo, N evento
        /// typedef pair<double,int> evento;



        colaEventos.encolar( * new Event(0.0, e_llega_msg_a_C1 ) );

        colaEventos.encolar( * new Event(0.0, e_llega_pkg_a_C1 ) );



        int cont = 1;

        /// Un ciclo del while corresponde a un evento
        while (!colaEventos.vacio()) {

            limpiarConsola();


            /// tomar el siguiente evento en la cola
            Event event = colaEventos.desencolar();

            /// reloj = tiempo del evento que va a ocurrir
            reloj = event.time;

            ejecutarEvento( event.id ); ///ejecuta el evento con la tabla de eventos


            /// si se desencola un evento que sobrepase la duración
            /// solicitada, entonces se detiene la simulación

            if( event.time > tiempoSimulacion) break;

            cont++;

            cout << "SIMULACION #"<<i+1<<":"<<endl<<endl;
            cout << "Evento #"<<cont<<endl;

            ///se guardan longitudes de colas para las estadísticas
            /*
            longitudA.push_back(A->colaArchivos1.size() + A->colaArchivos2.size());
            longitudB.push_back(B->colaArchivos1.size() + B->colaArchivos2.size());
            longitudC.push_back(C->colaArchivos1.size() + C->colaArchivos2.size());

            //longitudAV1.push_back(AV->cola1.size());
            //longitudAV2.push_back(AV->cola2.size());
            */

            cout << "\nReloj: "<< reloj << endl;
            /// cout << endl <<"La computadora con el token es la maquina: "<< TokenHolder->id << endl;

            cout << endl <<"Evento procesado actualmente: "<< eventoProcesado( event.id  ) << endl;

            /*
            cout << endl <<"Longitud cola archivos prioridad 1 de A: "<< A->colaArchivos1.size()<< endl;
            cout  <<       "Longitud cola archivos prioridad 2 de A: "<< A->colaArchivos2.size()<< endl << endl;

            cout << endl <<"Longitud cola archivos prioridad 1 de B: "<< B->colaArchivos1.size()<< endl;
            cout <<        "Longitud cola archivos prioridad 2 de B: "<< B->colaArchivos2.size()<< endl << endl;

            cout << endl <<"Longitud cola archivos prioridad 1 de C: "<< C->colaArchivos1.size()<< endl;
            cout <<        "Longitud cola archivos prioridad 2 de C: "<< C->colaArchivos2.size()<< endl;
            */

            cout << "Cantidad de eventos encolados: " << colaEventos.heap.size() << endl;

            if( lento) sleep(1000); //si se eligió la opción de ver la simulación en modo lento, se ejecuta un sleep de 1 segundo
        }

        cont = 0;

        double colaA = Est::promedioCola(&longitudA); //se calcula el promedio de la cola en cada simulación

        tpromcolaA += colaA;                          //se acumula para un promedio total

        double colaB = Est::promedioCola(&longitudB); //se hace lo mismo con el resto de las colas

        tpromcolaB += colaB;

        double colaC = Est::promedioCola(&longitudC);

        tpromcolaC += colaC;

        double cola1AV = Est::promedioCola(&longitudAV1);

        tpromcolaAV1 += cola1AV;

        double cola2AV = Est::promedioCola(&longitudAV2);

        tpromcolaAV2 += cola2AV;

        cout << "Estadisticas corrida #"<< i+1 << endl << endl;
        cout << "Longitud promedio de  cola en computadora A: " << colaA << endl << endl;
        cout << "Longitud promedio de cola en computadora B: " << colaB << endl << endl;
        cout << "Longitud promedio de cola en computadora C: " << colaC << endl << endl;
        cout << "Longitud promedio de cola 1 en servidor de antivirus: " << cola1AV << endl << endl;
        cout << "Longitud promedio de cola 2 en servidor de antivirus: " << cola2AV << endl << endl << endl;

        //double promPer = Est::promedioPermanencia(&permanencia); //se calcula el promedio de permanencia de un archivo por corrida

        ///se acumula para estadísticas totales
        /*
        tpromPer += promPer;
        cout << "Promedio de permanencia de un archivo en el sistema: " << promPer << " segundos." << endl << endl << endl;
        double promEnv = Est::promedioToken(&A->archivos_token, &B->archivos_token, &C-> archivos_token);

        tpromEnv += promEnv;

        cout << "Promedio de archivos enviados por turno del token: " << promEnv << endl << endl << endl;
        */

        colaEventos.vaciar(); //elimina eventos viejos que no se usarán en la siguiente corrida
        //getchar();//Espera un caracter para poder revisar datos entre corridas
        //getchar();

        /// Restaura todas las estructuras como nuevas para su uso
        /// en la siguiente corrida, si se está en la úlitma entonces no se hace

        if(i != numSim -1 ) restEst();
    }

    limpiarConsola();

    tpromcolaA = tpromcolaA / numSim;  //se calculan promedios totales
    tpromcolaB = tpromcolaB / numSim;
    tpromcolaC = tpromcolaC / numSim;
    tpromcolaAV1 = tpromcolaAV1 / numSim;
    tpromcolaAV2 = tpromcolaAV2 / numSim;
    tpromPer = tpromPer / numSim;
    tpromEnv = tpromEnv / numSim;
    tpromRev = tpromRev / numSim;

    cout << "ESTADISTICAS TOTALES: " << endl << endl;  //se muestran finalmente los promedios totales de todas las corridas

    cout<< "Longitud promedio total de cola en computadora A: " << tpromcolaA << endl << endl;
    cout<< "Longitud promedio total de cola en computadora B: " << tpromcolaB << endl << endl;
    cout<< "Longitud promedio total de cola en computadora C: " << tpromcolaC << endl << endl;
    cout<< "Longitud promedio total de cola 1 en servidor de antivirus: " << tpromcolaAV1 << endl << endl;
    cout<< "Longitud promedio total de cola 2 en servidor de antivirus: " << tpromcolaAV2 << endl << endl;

    cout << endl;

    cout << "Promedio de total permanencia de un archivo en el sistema: " << tpromPer <<" segundos." << endl<<endl;
    cout << "Promedio de total archivos enviados por turno del token: " << tpromEnv << endl<<endl;
    cout << "Promedio de total revisiones por archivo: " << tpromRev << endl<<endl;
}


void restEst() {
    colaMsgsC1.clear();
    colapkgsC1.clear();

    colaACKsC1.clear();

    colaMsgsC2.clear();

    colapkgsC3.clear();

    /*
    while(!AV->cola1.empty())     //las colas stl no tienen un clear directo, por lo que hubo que iterar
        AV->cola1.pop();

     while(!AV->cola2.empty())
        AV->cola1.pop();
*/

}



list <mensaje*> colaMsgsC1;
list <paquete*> colapkgsC1;

list <paquete*> colaACKsC1;

list <mensaje*> colaMsgsC2;

list <paquete*> colapkgsC3;
/// ------------------------------------------------

///                    EVENTOS

/// ------------------------------------------------

void llega_msg_a_C1                 () {}
void llega_pkg_a_C1                 () {

    if ( A->ocupada ) {

    }
    else {  ///  Si NO esta' ocupada comptra A
        A->ocupada = true;  //ahora lo esta'
        colapkgsC1.push_back(*new paquete);
    }

}
void termina_de_atender_pkg_en_C1_S1() {}
void termina_de_atender_pkg_en_C1_S2() {}
void llega_ACK_a_C1                 () {}
void llega_msg_malo_a_C1            () {}
void llega_msg_a_C2                 () {}
void termina_de_atender_msg_en_C2   () {}
void llega_pkg_a_C3                 () {}
void fin_procesa_pkg_en_C3          () {}
void se_activa_el_timer             () {}
