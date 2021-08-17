#include <iostream>
#include "analizadores/parser.h"
#include "analizadores/scanner.h"
#include "comandos/mkdisk.h"
using namespace std;

void leerEntrada(string entrada);

int main()
{
    // Disk discoPrueba;
    // discoPrueba.spath = "/discoPrueba.disk";
    // discoPrueba.size = 10;
    // discoPrueba.unit = 1;
    string entrada[4] = {"300", "ff", "k", "/discoPrueba1.disk"};
    RemoveDisk("/discoPrueba1.disk");
    CreateDisk(diskConstructor(entrada));
    leerEntrada("mkdisk -size = 300 -f = ff -u = k -path = /buenas/buenas/buenas.disk");
    leerEntrada("mkdisk -f = ff");
    leerEntrada("mkdisk -u = k");
    leerEntrada("mkdisk -path = /buenas/buenas/buenas.disk");

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