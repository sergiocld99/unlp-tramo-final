// Impresora única compartida por N personas
Bool libre = true;

process Persona[id: 0..N-1]
{
    Documento documento;

    <await(libre); libre = false; >
    imprimir(documento);
    libre = true;
}