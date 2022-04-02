/* A una cerealera van T camiones a descargarse trigo y M camiones a descargar maíz. Sólo hay
lugar para que 7 (siete) camiones a la vez descarguen pero no pueden ser más de 5 (cinco) del 
mismo tipo de cereal. Nota: sólo se pueden usar procesos que representen a los camiones. */

// Variables globales
sem lugar = 7;
sem trigo = 5;
sem maiz = 5;

// Procesos
process CamionTrigo[id: 0..T-1]
{
    P(lugar);       // Demorarse si hay 7 camiones en el lugar
    P(trigo);       // Demorarse si hay 5 camiones descargando trigo

    descargarTrigo();
    
    V(trigo);       // Liberar lugar para camiones de trigo
    V(lugar);       // Liberar lugar en cerealera
}

process CamionMaiz[id: 0..M-1]
{
    P(lugar);       // Demorarse si hay 7 camiones en el lugar
    P(maiz);        // Demorarse si hay 5 camiones descargando maiz

    descargarMaiz();
    
    V(maiz);        // Liberar lugar para camiones de maiz
    V(lugar);       // Liberar lugar en cerealera
}