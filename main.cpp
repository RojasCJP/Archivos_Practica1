#include <iostream>
#include "analizadores/parser.h"
#include "analizadores/scanner.h"
using namespace std;

void leerEntrada(string entrada);

int main()
{
    leerEntrada("mkdisk -size = 300");
    leerEntrada("mkdisk -f = ff");
    leerEntrada("mkdisk -u = k");
    leerEntrada("mkdisk -path = /buenas/buenas/buenas.disk");
    string prueba[4];
    prueba[0] = "hola";
    prueba[1] = "que";
    prueba[2] = "tal";
    prueba[3] = "<3";
    cout << prueba[0] << endl;

    return 0;
}

void leerEntrada(string entrada)
{
    // cout << "       leyendo--" << entrada  << endl;
    YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
    // cout << entrada << endl;
    if (yyparse() == 0)
    {
        // cout << "jalo" << endl;
    }
    else
    {
        cout << "!!! ocurrio un error al leer !! \n";
    }
}