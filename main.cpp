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

char* substr(char* arr, int begin, int len)
{
    char* res = new char[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

int main() {
//    char entrada[150];
//    while (entrada != "exit"){
//        cout<<"ingrese el comando que desea ejecutar"<<endl;
//        cin.getline(entrada,250);
//        string entradaAnalizar;
//        if(entrada[0]== 'e' && entrada[1] =='x'&& entrada[2] =='e' && entrada[3] == 'c'){
//            entradaAnalizar = substr(entrada,5,100);
//            cout<<entradaAnalizar<<endl;
//            leerArchivo(entradaAnalizar);
//            break;
//        }else{
//            cout<<entrada<<endl;
//            leerEntrada(entrada);
//        }
//    }
    leerArchivo("/entrada.txt");
//    leerEntrada("fdisk –size=30 -u=k –path=/discoPrueba1.disk –name=Particion1");
//    string entrada[4] = {"100", "k", "/discoPrueba1.disk", "ff"};
//    string entrada1[8] = {"30", "k", "/discoPrueba1.disk", "p", "ff", "", "#partition", ""};
//    string entrada2[8] = {"10", "k", "/discoPrueba1.disk", "p", "ff", "", "#partition1", ""};
//    string entrada3[8] = {"1", "k", "/discoPrueba1.disk", "l", "ff", "", "#partition2", ""};
//    string entrada5[8] = {"1", "k", "/discoPrueba1.disk", "l", "ff", "", "#partition3", ""};
//    string entrada4[8] = {"", "k", "/discoPrueba1.disk", "", "", "", "#partition1", "%10"};
//    string entradaMKFS1[3] = {"3fs", "fast", "891A"};
//    string entradaMKFS2[3] = {"2fs", "fast", "892A"};
//
//    RemoveDisk("/discoPrueba1.disk");
//    CreateDisk(diskConstructor(entrada));
//    makePartition(separarParams(entrada1));
//    makePartition(separarParams(entrada2));
//    makePartition(separarParams(entrada3));
//    makePartition(separarParams(entrada5));
//    makePartition(separarParams(entrada4));
//    Mount("/discoPrueba1.disk", "partition");
//    Mount("/discoPrueba1.disk", "partition1");
//    Unmount("891A");
//    mkfsMethod(separarParamsMKFS(entradaMKFS1));
//    mkfsMethod(separarParamsMKFS(entradaMKFS2));
//    loginn("root","123","891A");
//    addUser(active_sesion->id,"juan","asdf","root",false);
//    addGroup(active_sesion->id,"alumnos",false);
//    catFile("/users.txt", active_sesion->path,active_sesion->namePartition);
//    createFile("/prueba/file.txt",true,15,false);
//    editFile("/prueba/file.txt","hola",false);
//    createDirectory(true,active_sesion->id,"/home/junapa",false);
//
//    reportTree("/home/juanpa/Documents/tree.png","891A");
//    reporteDisk("/home/juanpa/Documents/disk.png", "891A");
//    reportBitmap(1, "891A","/home/juanpa/Documents/particion1.txt");
//    reportBitmap(0, "891A", "/home/juanpa/Documents/particion2.txt");
//    reportSuperBlock("891A", "/home/juanpa/Documents/sb.png");
//    reportInodes("891A", "/home/juanpa/Documents/inodos.png");
//    reportBlocks("891A", "/home/juanpa/Documents/bloques.png");

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
//todo tengo que hacer el mkdir, meterlo en la gramatica, el metodo es writedir
//todo tengo que hacer el touch, aunque creo que voy a tener que tunear esa onda