#include <iostream>
// #include "analizadores/parser.h"
// #include "analizadores/scanner.h"
#include "comandos/reportes.h"
using namespace std;

void leerEntrada(string entrada);

int main()
{
    string entrada[8] = {"10", "k", "/discoPrueba1.disk", "p", "wf", "", "#adios", ""};
    string entrada2[4] = {"300", "k", "/discoPrueba1.disk", "ff"};
    cout << "size " << separarParams(entrada).size << endl;
    cout << "units " << separarParams(entrada).units << endl;
    cout << "path " << separarParams(entrada).path << endl;
    cout << "type " << separarParams(entrada).type << endl;
    cout << "fit " << separarParams(entrada).fit << endl;
    cout << "delete " << separarParams(entrada).del << endl;
    cout << "name " << separarParams(entrada).name << endl;
    cout << "add " << separarParams(entrada).add << endl;
    RemoveDisk("/discoPrueba1.disk");
    CreateDisk(diskConstructor(entrada2));
    makePartition(separarParams(entrada));
    Mount("/discoPrueba1.disk", "adios");
    Unmount("891A");
    reporteDisk("/discoPrueba1.disk");
    // leerEntrada("rmdisk -path=/discoPrueba1.disk");

    return 0;
}

// void leerEntrada(string entrada)
// {
//     // cout << "       leyendo--" << entrada  << endl;
//     YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
//     // cout << entrada << endl;
//     if (yyparse() == 0)
//     {
//         // cout << "jalo" << endl;
//     }
//     else
//     {
//         cout << "!!! ocurrio un error al leer !! \n";
//     }
// }