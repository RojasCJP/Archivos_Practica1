#include <iostream>
#include "analizadores/parser.h"
#include "analizadores/scanner.h"
//#include "comandos/reportes.h"
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

    for (const auto &i: lines) {
        leerEntrada(i);
    }

    input_file.close();

    return EXIT_SUCCESS;
}

int main() {
    leerArchivo("/entrada.txt");
//    leerEntrada("fdisk –size=30 -u=k –path=/discoPrueba1.disk –name=Particion1");
    string entrada[4] = {"100", "k", "/discoPrueba1.disk", "ff"};
    string entrada1[8] = {"30", "k", "/discoPrueba1.disk", "p", "ff", "", "#adios", ""};
    string entrada2[8] = {"10", "k", "/discoPrueba1.disk", "p", "ff", "", "#adios1", ""};
    string entrada3[8] = {"1", "k", "/discoPrueba1.disk", "l", "ff", "", "#adios2", ""};
    string entrada5[8] = {"1", "k", "/discoPrueba1.disk", "l", "ff", "", "#adios3", ""};
    string entrada4[8] = {"", "k", "/discoPrueba1.disk", "", "", "", "#adios1", "%10"};
    string entradaMKFS1[3] = {"3fs", "fast", "891A"};
    string entradaMKFS2[3] = {"2fs", "fast", "892A"};

//    RemoveDisk("/discoPrueba1.disk");
//    CreateDisk(diskConstructor(entrada));
//    makePartition(separarParams(entrada1));
//    makePartition(separarParams(entrada2));
//    makePartition(separarParams(entrada3));
//    makePartition(separarParams(entrada5));
//    makePartition(separarParams(entrada4));
//    Mount("/discoPrueba1.disk", "particion1");
//    Mount("/discoPrueba1.disk", "adios1");
//    Unmount("891A");
//    mkfsMethod(separarParamsMKFS(entradaMKFS1));
//    mkfsMethod(separarParamsMKFS(entradaMKFS2));
//    reporteDisk("/discoprueba1.disk", "891A");
//    reportBitmap(1, "/discoprueba1.disk", "particion1", "/home/juanpa/Documents/particion1.txt");
//    reportBitmap(0, "/discoprueba1.disk", "particion1", "/home/juanpa/Documents/particion2.txt");
//    reportSuperBlock("/discoprueba1.disk", "particion1", "asdf");
//    reportInodes("/home/juanpa/Documents/discoprueba1.disk", "particion1", "/home/juanpa/Documents/inodos.png");
//    reportBlocks("/home/juanpa/Documents/discoprueba1.disk", "particion1", "/home/juanpa/Documents/bloques.png");
//    leerArchivo("/entradas/entrada.txt");
//    leerEntrada("mkdisk -path=/discoPrueba1.disk -size=300 -u=k");
//     leerEntrada("rmdisk -path=/discoPrueba1.disk");

    return 0;
}

void leerEntrada(string entrada) {
    // cout << "       leyendo--" << entrada  << endl;
    YY_BUFFER_STATE buffer = yy_scan_string(entrada.c_str());
    // cout << entrada << endl;
    if (yyparse() == 0) {
        // cout << "jalo" << endl;
    } else {
        cout << "!!! ocurrio un error al leer !! \n";
    }
}

//todo para hacer la ejecucion de comandos directos, lo unico que tengo que hacer es cambiar el string entrada en el leer entrada, tengo que poner un cin
// todo tengo que hacer prubas de add en las particiones, principalmente si no cabe, o hay que reposicionar