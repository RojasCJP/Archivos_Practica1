#include <iostream>

using namespace std;
string rpath = "/home/juanpa/Documents";

class Sesion {
public:
    char *user;
    char *path;
    char *namePartition;
    char *id;
    char *idUser;
    char *idGrp;

    Sesion() {
        this->user = NULL;
        this->path = NULL;
        this->namePartition = NULL;
        this->id = NULL;
        this->idGrp = NULL;
        this->idUser = NULL;
    }

    void clear() {
        user = NULL;
        path = NULL;
        namePartition = NULL;
        id = NULL;

        this->idGrp = NULL;
        this->idUser = NULL;
    }
};

extern Sesion *active_sesion;
enum Operation {
    MKDIRECTORY, MKFILE_PATH, MKFILE_SIZE, EMPTY, ADDUSER, ADDGRP, DELUSER, DELGRP,
    EDIT_FILE
};

class Journal {
public:
    Operation j_operation;
    time_t j_date;
    char *j_path;
    char *j_content;
    char *j_user;
    char *j_group;
    int j_size;
    int j_perms;
    bool j_boolean;

    Journal() {
        this->j_operation = EMPTY;
        //this->j_date = NULL;
        this->j_path = NULL;
        this->j_content = NULL;
        this->j_group = NULL;
        this->j_user = NULL;
        this->j_size = -1;
        this->j_perms = -1;
        this->j_boolean = false;
    }
};

struct INODO {
    int uid;
    int gid;
    time_t atime;
    time_t ctime;
    time_t mtime;
    int block[15];
    char type;
    int size;
    int permisos;
};

struct BLOCK_FILE {
    char content[64];
};

struct BLOCK_POINTER {
    int pointers[16];
};

struct CONTENT {
    char name[12];
    int inodo;
};

struct BLOCK_DIR {
    CONTENT content[4];
};

struct SuperBlock {
    int filesystemType;
    int inodesCount;
    int blocksCount;
    int freeBlocksCount;
    int freeInodesCount;
    time_t mtime;
    time_t umtime;
    int mntCount;
    int magic;
    int inodeSize;
    int blockSize;
    int firstInode;
    int firstBlock;
    int bmInodeStart;
    int bmBlockStart;
    int inodeStart;
    int blockStart;
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

struct EBR {
    int size;
    int start;
    int siguiente;
    string name;
};

struct ParamsFDisk {
    int size;
    char units;
    string path;
    char type;
    string fit;
    string del;
    string name;
    int add = 0;
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