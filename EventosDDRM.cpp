
void pasarToken()
{
    if(TokenHolder -> id == 'A')    //revisando el id sabe a cual otra computadora se le cederá el token inmediatamente
        {
        A->token = false;          //cambio de banderas en cada computadora
        B->token = true;
        B->tiempoTok = Token;      //se asigna primeramente el tiempo elegido por el usuario como tiempoTok a la computadora que recibe el token
        A->archivos_token.push_back(A->archivos); //como pasar token representa un cambio de turno, entonces se guarda el número de archivos enviados exitosamente de parte de la máquina que cede el token
        B->archivos = 0;           //ahora que se guardó el número de archivos enviados, entonces se resetea para que no se acumule para el siguiente turno
        TokenHolder = B;           //asigna a la computadora que recibe el token como TokenHolder
        archivo * envio = elegirArchivo(B); //se ve cual es el archivo que puede ser enviado en el momento actual
        if (envio != NULL)
        {
            enviado = envio;   //si fue posible elegir un archivo, este se guarda temporalmente en el archivo global
            double tiempoMandar = envio->tamano * 0.5; //se calcula la cantidad de tiempo requerida para enviar ese archivo
            double tiempoRestante = B->tiempoTok - tiempoMandar; //se asigna cuanto tiempo le quedará con el token a la máquina luego de enviar el archivo
            B->tiempoTok = tiempoRestante; //se asigna este tiempo como el nuevo tiempo del token
            evento ev = make_pair(reloj + tiempoMandar, 6); //se llama al evento llegada de archivo a la máquina respectiva
            colaEventos.encolar(ev);
        }
    }
    else if (TokenHolder->id == 'B')
    {                                           //se repite la secuencia anterior pero para cada diferente computadora
        B->token = false;                       //Es importante notar que A pasa a B, B pasa a C y C pasa a A
        C->token = true;
        C->tiempoTok = Token;
        B->archivos_token.push_back(B->archivos);
        C->archivos = 0;
        TokenHolder = C;
        archivo * envio = elegirArchivo(C);
        if (envio != NULL)
        {
            enviado = envio;
            double tiempoMandar = envio->tamano * 0.5;
            double tiempoRestante = C->tiempoTok - tiempoMandar;
            C->tiempoTok = tiempoRestante;
            evento ev = make_pair(reloj + tiempoMandar, 7);
            colaEventos.encolar(ev);

        }
    }
    else if (TokenHolder->id == 'C'){
        C->token = false;
        A->token = true;
        A->tiempoTok = Token;
        C->archivos_token.push_back(C->archivos);
        A->archivos = 0;
        TokenHolder = A;
        archivo * envio = elegirArchivo(A);
        if (envio != NULL)
        {
            enviado = envio;
            double tiempoMandar = envio->tamano * 0.5;
            double tiempoRestante = A->tiempoTok - tiempoMandar;
            A->tiempoTok = tiempoRestante;
            evento ev = make_pair(reloj + tiempoMandar, 5);
            colaEventos.encolar(ev);
        }
    }
    evento ne = make_pair(reloj + Token, 1);
    colaEventos.encolar(ne);
}

void llegaArchivoA()                  //evento que maneja la llegada de un archivo a A.
{
    archivo *arribo = new archivo;    //se crea nuevo archivo
    int numpac =  Prob::unif(1,128);   //se genera su tamaño en paquetes
    arribo->tamano = numpac;          //se asigna como tamaño
    arribo->tiempoArribo = reloj;     //se guarda el tiempo en el que llegó el archivo

    double prob = (double) rand()/RAND_MAX; //se genera probabilidad para definir la prioridad del archivo
    if (prob <= 0.25)                       //con una probabilidad de 25% será de prioridad uno y se guardará en esa cola
    {
        arribo->prioridad = 1;
        A->colaArchivos1.push_back(arribo);
        A->colaArchivos1.sort(ordenar_archivos()); //se aplica el ordenamiento para que quede en orden descendente de tamapo dentro de la cola
    }
    else                                   //si es prioridad 2 se hace lo mismo
    {
        arribo->prioridad = 2;
        A->colaArchivos2.push_back(arribo);
        A->colaArchivos2.sort(ordenar_archivos());
    }

    double he = reloj + Prob::exp(25.0);  //se genera el evento de la llegada del siguiente archivo a A
    evento ne = make_pair(he, 2);
    colaEventos.encolar(ne);

    return;
}

void llegaArchivoB() //sigue la misma lógica que llegaArchivoA
{
    archivo *arribo = new archivo;
    int numpac =  Prob::unif(1,128);
    arribo->tamano = numpac;
    arribo->tiempoArribo = reloj;

    double prob = (double) rand()/RAND_MAX;
    if (prob <= 0.25){
        arribo->prioridad = 1;
        B->colaArchivos1.push_back(arribo);
        B->colaArchivos1.sort(ordenar_archivos());
    }
    else
    {
        arribo->prioridad = 2;
        B->colaArchivos2.push_back(arribo);
        B->colaArchivos2.sort(ordenar_archivos());
    }

    double he = reloj + Prob::disB();
    evento ne = make_pair(he, 3);
    colaEventos.encolar(ne);

    return;
}

void llegaArchivoC()  //sigue la misma lógica que llegaArchivoA
{
    archivo *arribo = new archivo;
    int numpac =  Prob::unif(1,128);
    arribo->tamano = numpac;    arribo->tiempoArribo = reloj;

    double prob = (double) rand()/RAND_MAX;
    if (prob <= 0.25){
        arribo->prioridad = 1;
        C->colaArchivos1.push_back(arribo);
        C->colaArchivos1.sort(ordenar_archivos());
    }
    else
    {
        arribo->prioridad = 2;
        C->colaArchivos2.push_back(arribo);
        C->colaArchivos2.sort(ordenar_archivos());
    }
    double he = reloj + Prob::norm(5.0, 0.01);
    evento ne = make_pair(he, 4);
    colaEventos.encolar(ne);

    return;
}


void mandarArchivoA() //ya se encarga de cargar el archivo a la línea para ser enviado
{
    A->archivos++;    //registra que se manda el archivo exitosamente
    double t_arriboS = reloj + 0.25; //programa el arribo de este archivo al router
    evento llegadaS = make_pair(t_arriboS,8);
    colaEventos.encolar(llegadaS);
    archivo * envio = elegirArchivo(A); //elige el archivo que es el que se envia
    if(envio==NULL )  //si no se logró elegir un archivo..
    {
        colaEventos.elimCambio(); //se desprograma el evento "cambiar token" ya programado
        evento ev = make_pair(reloj, 1);//se programa pasar el token inmediatamente
        colaEventos.encolar(ev);
    }
    else
    {
        enviado = envio; //si si se eligió un archivo, se guarda en el archivo global y se siguen los mismos pasos usados en pasarToken para programar el proximo envío
        double tiempoMandar = envio->tamano * 0.5;
        double tiempoRestante = A->tiempoTok - tiempoMandar;
        A->tiempoTok = tiempoRestante;
        evento ev = make_pair(reloj + tiempoMandar, 5);
        colaEventos.encolar(ev);
    }

}

void mandarArchivoB()//sigue la misma lógica que mandarArchivoA
{
    B->archivos++;
    double t_arriboS = reloj + 0.25;
    evento llegadaS = make_pair(t_arriboS,8);
    colaEventos.encolar(llegadaS);
    archivo * envio = elegirArchivo(B);
    if(envio==NULL )
    {
        colaEventos.elimCambio();
        evento ev = make_pair(reloj, 1);
        colaEventos.encolar(ev);
    }
    else
    {
        enviado = envio;
        double tiempoMandar = envio->tamano * 0.5;
        double tiempoRestante = B->tiempoTok - tiempoMandar;
        B->tiempoTok = tiempoRestante;
        evento ev = make_pair(reloj + tiempoMandar, 6);
        colaEventos.encolar(ev);

    }

}

void mandarArchivoC()//sigue la misma lógica que mandarArchivoA
{
    C->archivos++;
    double t_arriboS = reloj + 0.25;
    evento llegadaS = make_pair(t_arriboS,8);
    colaEventos.encolar(llegadaS);
    archivo * envio = elegirArchivo(C);
    if(envio==NULL )
    {
        colaEventos.elimCambio();
        evento ev = make_pair(reloj, 1);
        colaEventos.encolar(ev);
    }
    else
    {
        enviado = envio;
        double tiempoMandar = envio->tamano * 0.5;
        double tiempoRestante = C->tiempoTok - tiempoMandar;
        C->tiempoTok = tiempoRestante;
        evento ev = make_pair(reloj + tiempoMandar, 7);
        colaEventos.encolar(ev);
    }

}


void llegaAServidor()      //maneja la llegada de archivos al servidor
{
   archivo * arc = enviado; //agarra el archivo enviado más recientemente, del archivo global
   double tiempoRevision = 0.0; //inicia tiempo de revision

   for(int i = 1; i < 4; i++)
   {
       double prob = (double) rand()/RAND_MAX; //genera probabilidad para ver cuantos virus tendrá el archivo
       if(prob <= 0.05)
        {
            arc->virus++;
            tiempoRevision += (double) arc->tamano/8*i; //se calcula cuantos virus tiene y cuanto durará en revisarse
        }
        else {
            break;
        }
   }
   if (AV->cola1.empty())// si no hay cola en el antivirus
    {
        AV->cola1.push(arc); //se encola el archivo a revisar
        evento ne = make_pair(reloj + tiempoRevision ,9); //se programa archivoRevisado para que desencole lo más antes posible al archivo
        colaEventos.encolar(ne);
   }
   else {
        AV->cola1.push(arc);//de haber cola, solo se encola el archivo y no se programa archivoRevisado
   }
}

void archivoRevisado()
{
    archivo * act = AV->cola1.front(); //obtiene el archivo más antiguo en cola
    int rev = calcularRevisiones(act); //averigua cuantas revisiones se hicieron al archivo
    virus.push_back(rev);              //se guarda el dato para las estadísitcas
    AV->cola1.pop();                   //se desencola "físicamente" el archivo
    if (act->virus < 3)                //se fija si no hay que desechar el archivo,
    {
        double tiempoRouter = (double) ((act->tamano)/64);//de no hacerlo se calcula lo que se duraría en mandarlo al router
        if(R->linea1 == false)         //si hay una línea desocupada se envía el archivo inmediatamente a esa línea y se coloca como ocupada
        {
            R->linea1 = true;
            evento ev = make_pair(reloj+tiempoRouter, 10); // se programa el evento soltar desde línea 1
            colaEventos.encolar(ev);
        }
        else if(R->linea2 == false) //pasa lo mismo que con R->linea1
        {
            R->linea2 = true;
            evento ev = make_pair(reloj+tiempoRouter, 11);
            colaEventos.encolar(ev);
        }
            AV->cola2.push(act); //se encola el archivo en la cola 2 del antivirus. La cola simbólica del router

    }
    if (AV->cola1.empty() == false) //si la cola no estaba vacía
    {
        act = AV->cola1.front();    //se obtiene el archivo más antiguo
        double tiempoRevision = 0.0;
        for(int i = 1; i <= act->virus; i++){
            tiempoRevision += (double) act->tamano/8*i; // se averigua el tiempo de revision de ese archivo para programar el proximo archivoRevisado
        }
        evento ne = make_pair(reloj + tiempoRevision ,9);
        colaEventos.encolar(ne);
    }
}


void soltarLinea1()     //paso final para liberar a un archivo por la línea 1
{
    if(!AV->cola2.empty())
    {
        archivo * act = AV->cola2.front(); // se obtiene el archivo más antiguo en la cola 2
        AV->cola2.pop();                   //se desencola
        double tp = reloj - act->tiempoArribo; //como es un archivo que se va a enviar exitosamente se guarda cuanto duró en el sistema para las estadísticas
        permanencia.push_back(tp);

        if (!AV->cola2.empty()) //ya mandado el archivo, revisa si todavía faltan archivos por mandar
        {
            act = AV->cola2.front();    //revisa el archivo más antiguo, pero no lo desencola
            double tiempoRouter = (double) act->tamano/64;      //calcula el tiempo que tardaría en transmitirse
            evento ev = make_pair(reloj + tiempoRouter, 10);    //programa un evento para el tiempo calculado
            colaEventos.encolar(ev);
        }
        else {
            R->linea1 = false;  //si no hay archivos por mandar, se desocupa la linea
        }
    }
    else
    {
        R->linea1 = false;  //si no se necesita mandar un archivo, se desocupa la linea
    }

}
void soltarLinea2()         //misma lógica que soltarLinea1
{
    if(!AV->cola2.empty())
    {
        archivo * act = AV->cola2.front();
        AV->cola2.pop();
        double tp = reloj - act->tiempoArribo;
        permanencia.push_back(tp);
        if (!AV->cola2.empty()){
            act = AV->cola2.front();
            double tiempoRouter = (double) act->tamano/64;
            evento ev = make_pair(reloj + tiempoRouter, 11);
            colaEventos.encolar(ev);
        }
        else{
            R->linea2 = false;
        }
    }
    else
    {
        R->linea2=false;
    }

} 
