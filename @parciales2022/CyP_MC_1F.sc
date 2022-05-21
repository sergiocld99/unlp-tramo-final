// -----------------------------------------
// EJERCICIO 1 - SENTENCIAS AWAIT
// -----------------------------------------

// Variables compartidas
Cola C;
bool pasar[N] = ([N] false);
int libres = 2;

Process Auto[id: 0..N-1]
{
  int aux = -1;
  
  < if (libres == 0) push(C, id);
  else { pasar[id] = true; libres--; } >
  
  < await(pasar[id]); >
  
  Pasar();
  
  < if (empty(C)) libres++;
  else pop(C, aux); >
  
  if (aux != 1) pasar[aux] = true;
}

// -----------------------------------------
// EJERCICIO 2 - SEMÁFOROS
// -----------------------------------------

// Variables compartidas
Cola C;
sem mutex = 1, turno[N] = ([N] 0);
bool libre = true;

Process Persona[id: 0..N-1]
{
  int idAux;
  
  P(mutex);
  if (libre)
  {
    libre = false;
    V(mutex);
  } else {
    push(C,id);
    V(mutex);
    P(turno[id]);
  }
  
  UsarCajero();
  
  P(mutex);
  if (empty(C)) libre = true;
  else {
    pop(C, idAux);
    V(turno[idAux]);
  }
  V(mutex);
}

// -----------------------------------------
// EJERCICIO 3 - MONITORES
// -----------------------------------------

Monitor Fila
{
  Cola listos;
  int libres = 0, esperando = 0;
  Cond espera;
  
  Procedure Esperar(idCajero : out int)
  {
    if (libres > 0) libres--;
    else {
      esperando++;
      wait(espera);
    }
    
    pop(listos, idCajero);
  }
  
  Procedure Llamar(idCajero : in int)
  {
    push(listos, idCajero);
    
    if (esperando == 0) libres++;
    else {
      esperando--;
      signal(espera);
    }
  }
}

Monitor Atencion[id: 0..1]
{
  text boleta, boletaSellada;
  Cond esperaCliente, esperaCajero;
  bool listo = false;
  
  Procedure Pagar(B : in text, BS : out text)
  {
    boleta = B;
    listo = true;
    signal(esperaCajero);   // avisar que entregué boleta
    wait(esperaCliente);    // espero por sellado
    BS = boletaSellada;
    signal(esperaCajero);   // avisar que me fui
  }
  
  Procedure Cobrar(B : out text)
  {
    if (not listo) wait(esperaCajero);
    B = boleta;
  }
  
  Procedure Terminar(BS : in text)
  {
    boletaSellada = BS;
    signal(esperaCliente);  // avisar que está sellada
    wait(esperaCajero);     // esperar por retirada
    listo = false;
  }
}

Process Persona[id: 0..N-1]
{
  text boleta, boletaSellada;
  int idCajero;
  
  Fila.Esperar(idCajero);
  Atencion[idCajero].Pagar(boleta, boletaSellada);
}

Process Cajero[id: 0..1]
{
  text boleta, boletaSellada;
  
  while (true)
  {
    Fila.Llamar(id);
    Atencion[id].Cobrar(boleta);
    boletaSellada = sellar(boleta);
    Atencion[id].Terminar(boletaSellada);
  }
}
