int contador = 0;
int num = N;
int arreglo[N] = ...;

process Revision[id: 0..M-1]
{
    if (arreglo[i] = N)
    {
        <contador := contador + 1 >
    }
}