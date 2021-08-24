#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <ctype.h>
#include "structs.h"
using namespace std;

int particion = 0;
Discos discos[5];
char letras[5] = {'A', 'B', 'C', 'D', 'E'};
//unit 1=>kb unit 2=>mb

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
    mbr.fit = 'f';
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
    cout << "fecha de creacion: " << asctime(gmtime(&mbr.creationDate));
    cout << "signature: " << mbr.diskSignature << endl;
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

void Mount(string spath, string name)
{
    if (spath == "" || name == "")
    {
        return;
    }
    string path = spath;
    for (int i = 0; i < spath.length(); i++)
    {
        path[i] = tolower(spath[i]);
    }

    int i = 0;
    string completePath = rpath + path;
    if (!exist_file(completePath))
    {
        cout << "disco no encontrado" << endl;
    }
    for (i = 0; i < 5; i++)
    {
        if (discos[i].state == 0 || discos[i].path == completePath)
        {
            break;
        }
    }
    if (i == 5)
    {
        cout << "no hay espacio para montar el disco" << endl;
        return;
    }

    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);
    cout << "------------------------------------------------disco encontrado------------------------------------------------" << endl;
    cout << "Fecha creacion: " << asctime(gmtime(&mbr.creationDate)) << endl;
    cout << "Signature: " << mbr.diskSignature << endl;
    cout << "Tamano: " << mbr.size << endl;
    cout << "Fit: " << mbr.fit << endl;
    string partitionName = "noname";
    int actualPartition = 5;
    for (int j = 0; j < 4; j++)
    {
        Partition partition = mbr.partition[j];
        if (partition.status == '1' && partition.name == name)
        {
            cout << "Particion: " << j << endl;
            cout << "Estado: " << partition.status << endl;
            cout << "Tamano: " << partition.size << endl;
            cout << "Fit: " << partition.fit << endl;
            cout << "Nombre: " << partition.name << endl;
            cout << "Start: " << partition.start << endl;
            partitionName = partition.name;
            actualPartition = j;
        }
    }
    if (!ExistName(partitionName, mbr))
    {
        cout << "no existe una particion con este nombre" << endl;
        return;
    }
    discos[i].state = 1;
    discos[i].path = completePath;
    int contadorParticion = 0;
    for (int x = 0; x < 10; x++)
    {
        mountedPartition mPartition = discos[i].partitions[x];
        if (mPartition.state == 0)
        {
            discos[i].partitions[x].state = 1;
            if (actualPartition < 5)
            {
                discos[i].partitions[x].name = mbr.partition[actualPartition].name;
            }
            else
            {
                cout << "error en la ejecucion del montaje" << endl;
            }
            discos[i].partitions[x].number = x + 1;
            discos[i].partitions[x].identificador = "89" + to_string(discos[i].partitions[x].number) + letras[i];
            contadorParticion = x;
            break;
        }
    }
    cout << "el identificador de su disco es: " << discos[i].partitions[contadorParticion].identificador << endl;
    cout << "Disco Montado" << endl;
}

void Unmount(string id)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (discos[i].partitions[j].identificador == id)
            {
                discos[i].partitions[j].state = 0;
                cout << "se desmonton la particion exitosamente" << endl;
            }
        }
    }
}

string getPathMount(string params[4])
{
    string datos[4];
    for (int i = 0; i < 4; i++)
    {
        datos[i] = params[i];
        if (datos[i][0] == '/' || datos[i][0] == '"')
        {
            return datos[i];
        }
    }
    return "";
}

string getNameMount(string params[4])
{
    string datos[4];
    for (int i = 0; i < 4; i++)
    {
        datos[i] = params[i];
        if (datos[i][0] != '/' && datos[i][0] != '"')
        {
            return datos[i];
        }
    }
    return "";
}