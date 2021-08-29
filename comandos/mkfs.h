
#include "fdisk.h"
#include <math.h>

using namespace std;

string getPathDisk(string idPartitionMounted) {
    Partition partition;
    string path;
    int size = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (discos[i].partitions[j].identificador == idPartitionMounted) {
                path = discos[i].path;
            }
        }
    }
    if (!exist_file(path)) {
        cout << "disco no encontrado" << endl;
        cout << "revisa la ruta: " << path << endl;
    }
    char sc[path.size() + 1];
    strcpy(sc, path.c_str());
    return sc;
}

int getSizePartition(string idPartition) {
    Partition partition;
    string path;
    mountedPartition partitionMounted;
    int size = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (discos[i].partitions[j].identificador == idPartition) {
                path = discos[i].path;
                partitionMounted = discos[i].partitions[j];
            }
        }
    }
    if (!exist_file(path)) {
        cout << "disco no encontrado" << endl;
        cout << "revisa la ruta: " << path << endl;
    }
    char sc[path.size() + 1];
    strcpy(sc, path.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);
    for (int i = 0; i < 4; ++i) {
        if (mbr.partition[i].name == partitionMounted.name) {
            partition = mbr.partition[i];
        }
    }
    size = partition.size;
    fclose(file);
    return size;
}

string getNamePartition(string idPartition) {
    Partition partition;
    string path;
    mountedPartition partitionMounted;
    string name;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (discos[i].partitions[j].identificador == idPartition) {
                path = discos[i].path;
                partitionMounted = discos[i].partitions[j];
            }
        }
    }
    if (!exist_file(path)) {
        cout << "disco no encontrado" << endl;
        cout << "revisa la ruta: " << path << endl;
    }
    char sc[path.size() + 1];
    strcpy(sc, path.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);
    for (int i = 0; i < 4; ++i) {
        if (mbr.partition[i].name == partitionMounted.name) {
            partition = mbr.partition[i];
        }
    }
    name = partition.name;
    fclose(file);
    return name;
}

double Ext2GetN(int tamano) {
    double N = 0;
    N = (tamano - sizeof(SuperBlock)) / (4 + sizeof(INODO) + (3 + sizeof(BLOCK_FILE)));
    return floor(N);
}

double Ext3GetN(int tamano) {
    double N = 0;
    N = (tamano - sizeof(SuperBlock) - (100 * sizeof(BLOCK_FILE))) / (4 + sizeof(INODO) + (3 * sizeof(BLOCK_FILE)));
    return floor(N);
}

void mkfsMethod(string id) {
    auto end = std::chrono::system_clock::now();
//todo tengo que ver que reciba los demas parametros para hacer las cosas con el ext3 y lo del fast y full que no se que es bien la verda
    string path = getPathDisk(id);
    string name = getNamePartition(id);
    int start = 0;
    char sc[path.size() + 1];
    strcpy(sc, path.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);
    for (int i = 0; i < 4; ++i) {
        if (mbr.partition[i].name == name) {
            start = mbr.partition[i].start;
        }
    }
    int N = 0;
    //todo aqui va lo del ext2 y ext3
    N = Ext2GetN(getSizePartition(id));
    SuperBlock SBnuevo;
    SBnuevo.filesystemType = 2; //todo esta es la ext entonces tambien tengo que tocarlo aqui
    SBnuevo.inodesCount = N;
    SBnuevo.blocksCount = 3 * N;
    SBnuevo.freeInodesCount = N;
    SBnuevo.freeBlocksCount = 3 * N;
    SBnuevo.mtime = std::chrono::system_clock::to_time_t(end);
    SBnuevo.umtime = std::chrono::system_clock::to_time_t(end);
    SBnuevo.inodeSize = sizeof(INODO);
    SBnuevo.blockSize = sizeof(BLOCK_FILE);
    SBnuevo.firstInode = 0;
    SBnuevo.firstBlock = 0;
    int aux = start + sizeof(SuperBlock);
    SBnuevo.bmInodeStart = aux;
    int aux2 = aux + N;
    SBnuevo.bmBlockStart = aux2;
    int aux3 = aux2 + N * sizeof(INODO);
    SBnuevo.inodeStart = aux3;
    SBnuevo.blockStart = aux3 + 3 * N * sizeof(BLOCK_FILE);
}
//todo tengo que jalar lo de la carpeta root




#ifndef PROYECTO1_MKFS_H
#define PROYECTO1_MKFS_H

#endif //PROYECTO1_MKFS_H
