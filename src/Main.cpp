#include "Main.h"
  

//--------------------------variables globales--------------------------------------


colaP colaEventos; //> La lista/cola de eventos

static Computadora
 *A = new Computadora, //struct representando cada una de las Computadoras, son globales
 *B = new Computadora,
 *C = new Computadora;


list <Msg*> colaMsgsC1;

list <Pkg*> colapkgsC1;

list <ACK*> colaACKsC1;

list <Msg*> colaMsgsC2;

list <Pkg*> colapkgsC3;


/// ------------------------------------------------

///                    EVENTOS

/// ------------------------------------------------

/* E1 */
void llega_msg_a_C1                 () {
    Msg* m new Msg();
    if ( A->ocupada ) {
        colaMsgsC1.push_back(m);
    }
    else {  ///  Si NO esta' ocupada PC A
        A->ocupada = true;  //atender ya mismo

        double t_atender =  Prob::exp(0.5);

        double tiempo = Reloj + t_atender;  ///   ???????????????????????????????
        colaEventos.encolar(  new Event( tiempo , e_termina_de_atender_pkg_en_C1_S2 ) );

    }
    // llegada de siguiente msg
    double tiempo = Reloj + Prob::norm(4, 0.01);
    colaEventos.encolar(  new Event( tiempo , e_llega_msg_a_C1 ) );
}

/* E2 */
int secuenciaC1=0,
    EnviadosSinAck = 0; // enviados sin ack
bool hayCampoVentana() {
    // tamanyo ventana = 4
    if ( EnviadosSinAck < 4 ) {
        // se asume que cuando esta Fn retorne, 
        // se envio un pkg sin ack
        EnviadosSinAck++;
        return true;
    }
    return false;

}

void llega_pkg_a_C1                 () {
    // ------------------------------------
    Pkg* p = new Pkg();
    p->id = secuenciaC1;

    secuenciaC1++;
    if (secuenciaC1>99) { secuenciaC1 = 0; }
    // ------------------------------------
    
    if ( A->ocupada ) {
        colapkgsC1.push_back(p);
    }
    else {  ///  Si NO esta' ocupada PC A
        if ( hayCampoVentana() ) {
            A->ocupada = true;  //atender ya mismo

            //cua'nto va a durar en atenderlo
            double t_atender =  Prob::exp(0.5);
            double tiempo = Reloj + t_atender;  
            colaEventos.encolar(  new Event( tiempo , e_termina_de_atender_pkg_en_C1_S2 ) );
        }
        else colapkgsC1.push_back(p);
    }
    // llegada de siguiente pkg
    double tiempo = Reloj + Prob::unif (3, 7);
    colaEventos.encolar(  new Event( tiempo , e_llega_pkg_a_C1 ) );

}
/* E3 */
void termina_de_atender_msg_en_C1_S1() {
    A->ocupada = false;  // ??????????  A S1   ,   A S2
    //tirar tiempo de atencion (funcion rara que hace falta)
    double tiempo = Reloj + 3;
    //mandar mensaje a C2 y desencolar mensaje de C1
    colaEventos.encolar(  new Event( tiempo , e_llega_msg_a_C2 ) );
}
/* E4 */
void termina_de_atender_pkg_en_C1_S2() {
    // el tiempo de propagacion es 2
    double tiempo = Reloj + 2;
    colaEventos.encolar(  new Event( tiempo , e_llega_pkg_a_C3 ) );
}
/* E5 */
void llega_ACK_a_C1                 () {}
/* E6 */
void llega_msg_malo_a_C1            () {}
/* E7 */
void llega_msg_a_C2                 () {}
/* E8 */
/* igual que E1*/
void termina_de_atender_msg_en_C2   () {}
/* E9 */
/* Tirar probabilidad de que mensaje sea malo
    tirar tiempo de atencion
    si hay error en mensaje, devuelve mensaje a c1
    si no hay error mata mensaje*/
void llega_pkg_a_C3                 () {
    
    if ( C->ocupada ) {

        //colapkgsC3.push_back( pkg );

    }
    else {  ///  Si NO esta' ocupada PC
        C->ocupada = true;  //ahora lo esta'

        double t_atender =  Prob::norm(1.5, 0.01);

        double tiempo = Reloj + t_atender;

        colaEventos.encolar(  new Event( tiempo , e_fin_procesa_pkg_en_C3 ) );
    }

    // si llego un pkg con tiempo menor, debe ser atendido
    if ( ! colapkgsC3.empty() ) {
        cout << "colapkgsC3 no est'a vac'ia TODO ";
        double tiempo = Reloj + Prob::norm(4, 0.01);
        colaEventos.encolar(  new Event( tiempo , e_llega_pkg_a_C3 ) );
    }
    // else {} E9 = infinito
}
/* E10 */
int secuenciaC3 = 0;
void fin_procesa_pkg_en_C3          () {
    secuenciaC3++;
    if (secuenciaC3>99) { secuenciaC3 = 0; }

    Pkg * pt = colapkgsC3.back() ;
    
    C->ocupada = false;
}
/* E11 */
void se_activa_el_timer             () {}


//-------------------------------------------------------------------------------------------------------
///--------------------------ESTADISTICAS----------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------


///estructuras de datos usadas para recolectar estadisticas a desplegar durante y despu�s de la simulaci�n
list <int> longitudA;     //guarda la longitud de la cola de la Computadora A (incluyendo ambas prioridades) en cada paso de la simulacion
list <int> longitudB;     //guarda la longitud de la cola de la Computadora B (incluyendo ambas prioridades) en cada paso de la simulacion
list <int> longitudC;     //guarda la longitud de la cola de la Computadora C (incluyendo ambas prioridades) en cada paso de la simulacion
list <int> longitudAV1;   //guarda la longitud de la cola 1 del antivirus en cada paso de la simulacion
list <int> longitudAV2;   //guarda la longitud de la cola 2 del antivirus en cada paso de la simulacion



//-------------------------------------------------------------------------------------------------------
///METODOS AUXILIARES PARA LA IMPLEMENTACION DE LOS EVENTOS----------------------------------------------
//-------------------------------------------------------------------------------------------------------

/*

    /**
    metodo auxiliar a los mandarArchivo de todas las Computadoras.
    Se encarga de escoger al proximo archivo
    a enviar. Toma como criterio principal escoger al archivo
    m�s grande que se pueda enviar en el presente
    turno del token, as� como la prioridad
    * -- /
archivo* elegirArchivo(Computadora* com) {
    list<archivo *>::iterator it;
    archivo* elegido = NULL;

    for(it = com->colaArchivos1.begin(); it != com->colaArchivos1.end(); ++it) {
                                                        //primero se recorre la lista de prioridad 1. Ambas estan ordenadas
                                                        //descendentemente en orden de tama�o del archivo, por lo que al
        double tiempoArc = (*it)->tamano * 0.5;         //encontrar un archivo que pueda ser enviado en el turno del token,
        if(com->tiempoTok - tiempoArc >= 0)             //este ser� el m�s grande posible que pueda ser enviado. Si ninguno
        {                                               //cabe en el turno del token entonces se pasa a la lista de prioridad 2
            elegido = *it;                              //y se itera sobre ella de la misma manera
            com->colaArchivos1.erase(it);
            break;
        }
    }
    //solo se busca en la cola de prioridad 2 si en la cola 1 no se logr�
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

    return elegido;  //se devuelve el archivo escogido, d�ndose la posibilidad de que sea nulo, lo que significar�a que: no hab�a archivos en ninguna cola o
}                    //ningun archivo en las colas pod�a ser enviado en el presente turno del token

int calcularRevisiones(archivo * arc) //metodo auxiliar para calcular el numero de revisiones hechas sobre el archivo bas�ndose en la cantidad de virus que tenga
{
    int rev = 0;
    if(arc->virus == 0) //si no ten�a ni un virus entonces solo se revis� una vez
    {
        rev = 1;
    }
    else if ( arc ->virus == 1) //si ten�a 1 entonces se dieron 2 revisiones
    {
        rev = 2;
    }
    else if (arc->virus == 2 || arc->virus == 3) // con 2 o 3 se dan 3 revisiones
    {
        rev = 3;
    }
    return rev;  //devuelve cantidad de revisiones para guardarlo para las estad�sticas
}

void restEst()                    //como las structs usadas son globales y la simulaci�n puede correrse varias veces entonces se requiere
{                                 //poder restaurar los valores originales de cada struct para evitar acumulaci�n err�nea de archivos entre corridas
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

static const char* eventoProcesado(int i) {
    ///> nombres de los eventos
    const char* eventos [] = {
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
    return eventos[i-1];
}


//-------------------------------------------------------------

//-------------------------------------------------------------
//-------------------------------------------------------------


/**
 * Llama a cada evento
 * recibe un int des-encolado de la cola de eventos
 * para decidir cual evento ejecutar a continuacion
 */
void ejecutarEvento(int i) {
    printf("\n--Ejecutando evento %s\n", eventoProcesado(i) );
    switch (i){
        case 1 : {llega_msg_a_C1                 (); break;}
        case 2 : {llega_pkg_a_C1                 (); break;}
        case 3 : {termina_de_atender_msg_en_C1_S1(); break;}
        case 4 : {termina_de_atender_pkg_en_C1_S2(); break;}
        case 5 : {llega_ACK_a_C1                 (); break;}
        case 6 : {llega_msg_malo_a_C1            (); break;}
        case 7 : {llega_msg_a_C2                 (); break;}
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

void restEst() {
    colaMsgsC1.clear();
    colapkgsC1.clear();

    colaACKsC1.clear();

    colaMsgsC2.clear();

    colapkgsC3.clear();
}


int main() {
    srand(time(NULL));
    ///> semilla, necesaria para las funciones de probabilidad

    //for (int i=0; i<5;i++) cout<< Prob::unif(3, 7)<<endl;

    double
        numSim,
        tiempoSimulacion ;

    cout << "\nIngrese el numero de veces que quiere correr la simulacion\n>> "; cin >> numSim;
    cout << "\nIngrese la duracion maxima de la simulacion [en segundos]\n>> " ; cin >> tiempoSimulacion;

    char  resp;
    cout << endl << "Desea ver simulacion en modo lento ( s / n )? \n\n" ;

    scanf(" %c", &resp);
    bool lento = ( resp == 's') ? true: false;

    while ( getchar() != '\n' ); /// flush

    


    /*
    while ( getchar() != '\n' ); /// flush


    char c = getchar();


    bool lento = (c == 's') ? true: false;

    while ( getchar() != '\n' ); /// flush*/


    //while ((c = getchar()) != '\n' && c != EOF);





    double tpromcolaA = 0;  ///variables para guardar promedios totales
    double tpromcolaB = 0;
    double tpromcolaC = 0;
    double tpromcolaAV1 = 0;
    double tpromcolaAV2 = 0;
    double tpromPer = 0;
    double tpromEnv = 0;
    double tpromRev = 0;


    for(int i = 0; i < numSim; i++) {

        /** la simulacioin inicia con los eventos
            1 y 2,  llega msg a c1 y
                    llega pkg a c1
        */

        ///             tiempo, N evento
        /// typedef pair<double,int> evento;



        colaEventos.encolar(  new Event(0.0, e_llega_msg_a_C1 ) );

        colaEventos.encolar(  new Event(0.0, e_llega_pkg_a_C1 ) );



        int cont = 1;

        /// Un ciclo del while corresponde a un evento
        while (!colaEventos.vacio()) {

            limpiarConsola();


            /// tomar el siguiente evento en la cola
            Event event = colaEventos.desencolar();

            /// Reloj = tiempo del evento que va a ocurrir
            Reloj = event.time;

            if( event.time > tiempoSimulacion) {
                cout << "\nse ha acabado el tiempo\n";
                break;
            };



            cout << "SIMULACION #"  <<  i+1     <<":"<<endl<<endl;
            cout << "\nEvento #"    <<  cont  ;
            cout << "\t|\tReloj: "     <<  Reloj  << endl ;
            cout << "\nCantidad de eventos encolados: " << colaEventos.heap.size() << endl;
            ejecutarEvento( event.id ); ///ejecuta el evento con la tabla de eventos


            /// cout << endl <<"La Computadora con el token es la maquina: "<< TokenHolder->id << endl;



            /// si se desencola un evento que sobrepase la duraci�n
            /// solicitada, entonces se detiene la simulaci�n


            ///se guardan longitudes de colas para las estad�sticas
            /*
            longitudA.push_back(A->colaArchivos1.size() + A->colaArchivos2.size());
            longitudB.push_back(B->colaArchivos1.size() + B->colaArchivos2.size());
            longitudC.push_back(C->colaArchivos1.size() + C->colaArchivos2.size());

            //longitudAV1.push_back(AV->cola1.size());
            //longitudAV2.push_back(AV->cola2.size());
            */


            /*
            cout << endl <<"Longitud cola archivos prioridad 1 de A: "<< A->colaArchivos1.size()<< endl;
            cout  <<       "Longitud cola archivos prioridad 2 de A: "<< A->colaArchivos2.size()<< endl << endl;

            cout << endl <<"Longitud cola archivos prioridad 1 de B: "<< B->colaArchivos1.size()<< endl;
            cout <<        "Longitud cola archivos prioridad 2 de B: "<< B->colaArchivos2.size()<< endl << endl;

            cout << endl <<"Longitud cola archivos prioridad 1 de C: "<< C->colaArchivos1.size()<< endl;
            cout <<        "Longitud cola archivos prioridad 2 de C: "<< C->colaArchivos2.size()<< endl;
            */



            /// si se eligi� la opci�n de ver la simulaci�n en modo lento,
            /// se ejecuta un sleep de 2 segundo
            printf("\n[ presione ENTER para continuar ]");

            if( lento) {
                fflush(stdin);
                while ( getchar() != '\n');
                fflush(stdin);
            }
            cont++;
        }

        cout << "\nfin while\n";

        cont = 0;

        double colaA = Est::promedioCola(&longitudA); //se calcula el promedio de la cola en cada simulaci�n

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
        cout << "Longitud promedio de  cola en Computadora A: " << colaA << endl << endl;
        cout << "Longitud promedio de cola en Computadora B: " << colaB << endl << endl;
        cout << "Longitud promedio de cola en Computadora C: " << colaC << endl << endl;
        cout << "Longitud promedio de cola 1 en servidor de antivirus: " << cola1AV << endl << endl;
        cout << "Longitud promedio de cola 2 en servidor de antivirus: " << cola2AV << endl << endl << endl;

        //double promPer = Est::promedioPermanencia(&permanencia); //se calcula el promedio de permanencia de un archivo por corrida

        ///se acumula para estad�sticas totales
        /*
        tpromPer += promPer;
        cout << "Promedio de permanencia de un archivo en el sistema: " << promPer << " segundos." << endl << endl << endl;
        double promEnv = Est::promedioToken(&A->archivos_token, &B->archivos_token, &C-> archivos_token);

        tpromEnv += promEnv;

        cout << "Promedio de archivos enviados por turno del token: " << promEnv << endl << endl << endl;
        */

        colaEventos.vaciar(); //elimina eventos viejos que no se usar�n en la siguiente corrida
        //getchar();//Espera un caracter para poder revisar datos entre corridas
        //getchar();

        /// Restaura todas las estructuras como nuevas para su uso
        /// en la siguiente corrida, si se est� en la �litma entonces no se hace

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

    cout<< "Longitud promedio total de cola en Computadora A: " << tpromcolaA << endl << endl;
    cout<< "Longitud promedio total de cola en Computadora B: " << tpromcolaB << endl << endl;
    cout<< "Longitud promedio total de cola en Computadora C: " << tpromcolaC << endl << endl;
    cout<< "Longitud promedio total de cola 1 en servidor de antivirus: " << tpromcolaAV1 << endl << endl;
    cout<< "Longitud promedio total de cola 2 en servidor de antivirus: " << tpromcolaAV2 << endl << endl;

    cout << endl;

    cout << "Promedio de total permanencia de un archivo en el sistema: " << tpromPer <<" segundos." << endl<<endl;
    cout << "Promedio de total archivos enviados por turno del token: " << tpromEnv << endl<<endl;
    cout << "Promedio de total revisiones por archivo: " << tpromRev << endl<<endl;
}

    /*
    while(!AV->cola1.empty())     //las colas stl no tienen un clear directo, por lo que hubo que iterar
        AV->cola1.pop();

     while(!AV->cola2.empty())
        AV->cola1.pop();
*/

