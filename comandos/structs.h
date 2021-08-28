#include <iostream>

using namespace std;
string rpath = "/home/juanpa/Documents";

struct INODO {};
struct BLOCK {};
struct BLOCK_DIR {};

struct SuperBlock {
    int fileSystemType;
    int inodesCount;
    int blocksCount;
    int inodoSize;
    int blockSize;
    int firstInodo;
    int firstBlock;
    int bitMapInodoStart;
    int bitMabBlockStart;
    int inodoStart;
    int blockStart;
    int journal;
};

struct Disk {
    string spath;
    int size;
    int unit;
};

struct mountedPartition {
    int number;
    string identificador;
    int state;
    int type = -1;
    string name;
};

struct Discos {
    string path;
    char letra;
    int state;
    mountedPartition partitions[10];
};

struct Partition {
    char status;
    char type;
    char fit;
    int start;
    int size;
    char name[16];
};

struct MBR {
    int size;
    time_t creationDate;
    int diskSignature;
    char fit;
    Partition partition[4];
};

struct ParamsFDisk {
    int size;
    char units;
    string path;
    char type;
    string fit;
    string del;
    string name;
    int add;
};

bool ExistName(string name, MBR mbr) {
    for (int i = 0; i < 4; i++) {
        if (mbr.partition[i].name == name) {
            return true;
        }
    }
    return false;
}

bool exist_file(string &name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}