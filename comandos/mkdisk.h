#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
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