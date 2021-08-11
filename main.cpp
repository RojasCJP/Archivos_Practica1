#include <iostream>
#include "extras/presentacion.h"
#include "analizadores/parser.h"
#include "analizadores/scanner.h"
using namespace std;

void leerEntrada(string entrada);

int main()
{
    leerEntrada("mkdisk -size = 300");
    return 0;
}

void leerEntrada(string entrada){
    // cout << "       leyendo--" << entrada  << endl;
    YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
    cout<<entrada<<endl;
    if (yyparse() == 0)
    {
        cout<<"jalo"<<endl;

    }else{
        cout << "!!! ocurrio un error al leer !! \n";
    }
}