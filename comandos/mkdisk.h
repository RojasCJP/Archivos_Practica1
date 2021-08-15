#include <iostream>
#include <studio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
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
    Partition partition[4]
};
string path = "/home/juanpa/Documents";
//unit 1=>kb unit 2=>mb
struct Disk
{
    string path;
    int size;
    int unit;
};
void CreateDisk(Disk disk)
{
    cout << "creacion de disco" << endl;
    string completePath = path + disk.path;
    char diskc[disk.size() + 1];
    strcpy(diskc, disk.c_str());
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
    fwirte("\0", 1, 1, file);
    fseek(file, tamano, SEEK_SET);
    fwrite("\0", 1, 1, file);

    MBR mbr;
    mbr.tamano = tamano;
    mbr.diskSignature = rand() % 1000;
    mbr.creationDate = time();
    for (int i = 0; i < 4; i++)
    {
        mbr.partition[i].status = '0';
        mbr.partition[i].size = 0;
        mbr.partition[i].fit = 'f';
        mbr.partition[i].start = tamano;
        strcpy(mbr.partition[i].name, "")
    }

    cout << "disco creado" << endl;
    cout << "fecha de creacion: " << asctime(gmtime(&mbr.creationDate)) << endl;
    cout << "tamano: " << mbr.size << endl;
    cout << "fit: " << mbr.fit << endl;
    fseek(file, 0, SEEK_SET);
    fwrite(&mrb, sizeof(MBR), 1, file);
    fclose(file);
}