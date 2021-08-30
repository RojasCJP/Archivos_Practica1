
#include "fdisk.h"
#include <math.h>

using namespace std;

struct ParamsMKFS {
    string id;
    string type;
    int fs;
};

ParamsMKFS separarParamsMKFS(string params[3]) {
    ParamsMKFS paramsFinal;
    string datos[3];
    for (int i = 0; i < 3; i++) {
        datos[i] = params[i];
        for (int j = 0; j < params[i].length(); j++) {
            datos[i][j] = tolower(params[i][j]);
        }
        if (datos[i][0] == '8') {
            paramsFinal.id = params[i];
        } else if (datos[i][0] == '2' || datos[i][0] == '3') {
            if (datos[i][0] == '3') {
                paramsFinal.fs = 3;
            } else {
                paramsFinal.fs = 2;
            }
        } else if (datos[i] == "fast" || datos[i] == "full") {
            paramsFinal.type = datos[i];
        } else {
            cout << "ingreso mal sus datos para formatear por favor revise la entrada" << endl;
        }

    }
    return paramsFinal;
}

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

void mkfsMethod(ParamsMKFS params) {
    string id = params.id;
    int fs = params.fs;
    auto end = std::chrono::system_clock::now();
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
    if (fs == 2) {
        N = Ext2GetN(getSizePartition(id));
    } else {
        N = Ext3GetN(getSizePartition(id));
    }
    SuperBlock SBnuevo;
    SBnuevo.filesystemType = fs;
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

    if (params.type == "full") {
        fseek(file, start, SEEK_SET);
        for (int i = 0; i < getSizePartition(id); ++i) {
            fwrite("\0", 1, 1, file);
        }
    }

    fseek(file, start, SEEK_SET);
    fwrite(&SBnuevo, sizeof(SuperBlock), 1, file);
    fclose(file);
    cout << "se formateo la particion" << endl;
}

//todo tengo que jalar lo de la carpeta root




#ifndef PROYECTO1_MKFS_H
#define PROYECTO1_MKFS_H

#endif //PROYECTO1_MKFS_H
