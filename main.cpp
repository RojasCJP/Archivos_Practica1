#include <iostream>
// #include "analizadores/parser.h"
// #include "analizadores/scanner.h"
#include "comandos/reportes.h"
#include <fstream>
#include <vector>

using namespace std;
using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;

void leerEntrada(string entrada);

int leerArchivo(string nombreArchivo) {
    string rootPath = "/home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1";
    string filename(rootPath + nombreArchivo);
    vector<string> lines;
    string line;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    while (getline(input_file, line)) {
        lines.push_back(line);
    }

    for (const auto &i: lines) cout << i << endl;

    input_file.close();

    return EXIT_SUCCESS;
}

int main() {
    cout << "a ver si ahorita si se le antoja jalar" << endl;
    string entrada[4] = {"300", "k", "/discoPrueba1.disk", "ff"};
    string entrada1[8] = {"10", "k", "/discoPrueba1.disk", "e", "ff", "", "#adios", ""};
    string entrada2[8] = {"10", "k", "/discoPrueba1.disk", "e", "ff", "", "#adios1", ""};
    string entrada3[8] = {"10", "k", "/discoPrueba1.disk", "e", "ff", "", "#adios2", ""};
    string entrada4[8] = {"", "k", "/discoPrueba1.disk", "", "", "", "#adios2", "%10"};
    string entradaMKFS1[3] = {"3fs", "fast", "891A"};
    string entradaMKFS2[3] = {"2fs", "fast", "892A"};

    RemoveDisk("/discoPrueba1.disk");
    CreateDisk(diskConstructor(entrada));
    makePartition(separarParams(entrada1));
    makePartition(separarParams(entrada2));
    makePartition(separarParams(entrada3));
    makePartition(separarParams(entrada4));
    Mount("/discoPrueba1.disk", "adios");
    Mount("/discoPrueba1.disk", "adios1");
//    Unmount("891A");
    mkfsMethod(separarParamsMKFS(entradaMKFS1));
    mkfsMethod(separarParamsMKFS(entradaMKFS2));
    reporteDisk("/discoPrueba1.disk", "891A");
    leerArchivo("/entradas/entrada.txt");
//    leerEntrada("mkdisk -path=/discoPrueba1.disk -size=300 -u=k");
//     leerEntrada("rmdisk -path=/discoPrueba1.disk");

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

//todo para hacer la ejecucion de comandos directos, lo unico que tengo que hacer es cambiar el string entrada en el leer entrada, tengo que poner un cin
//todo hacer el exec