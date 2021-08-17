#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <ctype.h>
using namespace std;

struct Partition
{
    char status;
    char type;
    char fit;
    int start;
    int size;
    char name[16];
};

struct MBR
{
    int size;
    time_t creationDate;
    int diskSignature;
    char fit;
    Partition partition[4];
};
string rpath = "/home/juanpa/Documents";
//unit 1=>kb unit 2=>mb
struct Disk
{
    string spath;
    int size;
    int unit;
};
void CreateDisk(Disk disk)
{
    cout << "creacion de disco" << endl;
    string completePath = rpath + disk.spath;
    char diskc[disk.size + 1];
    strcpy(diskc, completePath.c_str());
    FILE *file = NULL;
    file = fopen(diskc, "r");
    if (file != NULL)
    {
        cout << "El disco que desea crear ya existe" << endl;
        cout << "se encuentra en la ubicacion " << completePath << endl;
        return;
    }
    int tamano;

    if (disk.unit = 1)
    {
        tamano = disk.size * 1024;
    }
    else
    {
        tamano = disk.size * 1024 * 1024;
    }

    file = fopen(diskc, "wb");
    fwrite("\0", 1, 1, file);
    fseek(file, tamano, SEEK_SET);
    fwrite("\0", 1, 1, file);

    auto end = std::chrono::system_clock::now();

    MBR mbr;
    mbr.size = tamano;
    mbr.diskSignature = rand() % 1000;
    mbr.creationDate = std::chrono::system_clock::to_time_t(end);
    for (int i = 0; i < 4; i++)
    {
        mbr.partition[i].status = '0';
        mbr.partition[i].size = 0;
        mbr.partition[i].fit = 'f';
        mbr.partition[i].start = tamano;
        strcpy(mbr.partition[i].name, "");
    }

    cout << "disco creado" << endl;
    cout << "fecha de creacion: " << asctime(gmtime(&mbr.creationDate)) << endl;
    cout << "tamano: " << mbr.size << endl;
    cout << "fit: " << mbr.fit << endl;
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, file);
    fclose(file);
}

void RemoveDisk(string subpath)
{
    string subpathlower = subpath;
    for (int i = 0; i < subpath.length(); i++)
    {
        subpathlower[i] = tolower(subpath[i]);
    }
    string fullPath = rpath + subpathlower;

    FILE *file = NULL;
    file = fopen(fullPath.c_str(), "r");
    if (file != NULL)
    {
        remove(fullPath.c_str());
        system("pause");
        cout << "se completo la eliminacion" << fullPath << endl;
    }
    else
    {
        cout << "el archivo no existe" << fullPath << endl;
    }
}

Disk diskConstructor(string datosEntrada[4])
{
    string path;
    int size;
    int unit;
    int fit;
    string datos[4];
    //fit 1 = > bf 2 = > ff 3 = > wf

    for (int i = 0; i < 4; i++)
    {
        datos[i] = datosEntrada[i];
        for (int j = 0; j < datosEntrada[i].length(); j++)
        {
            datos[i][j] = tolower(datosEntrada[i][j]);
        }
        cout << datos[i] << endl;
        if (datos[i][0] == '/' || datos[i][0] == '"')
        {
            path = datos[i];
        }
        else if (datos[i][0] == '0' || datos[i][0] == '1' || datos[i][0] == '2' || datos[i][0] == '3' || datos[i][0] == '4' || datos[i][0] == '5' || datos[i][0] == '6' || datos[i][0] == '7' || datos[i][0] == '8' || datos[i][0] == '9')
        {
            size = stoi(datos[i]);
        }
        else if (datos[i] == "ff" || datos[i] == "bf" || datos[i] == "wf")
        {
            if (datos[i] == "bf")
            {
                fit = 1;
            }
            else if (datos[i] == "ff")
            {
                fit = 2;
            }
            else
            {
                fit = 3;
            }
        }
        else
        {
            if (datos[i] == "m")
            {
                unit = 2;
            }
            else
            {
                unit = 1;
            }
        }
    }
    Disk discoPrueba;
    discoPrueba.spath = path;
    discoPrueba.size = size;
    discoPrueba.unit = unit;
    return discoPrueba;
}
