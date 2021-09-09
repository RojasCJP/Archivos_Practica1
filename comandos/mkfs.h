
#include "fdisk.h"
#include <math.h>
#include <sstream>

void createUsers();

using namespace std;

struct ParamsMKFS {
    string id;
    string type;
    int fs;
};

void loginn(char user[], char pass[], char id[]);

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

void writeBitmap(int cantInodos, int init, string path) {
    int contador = 0;
    FILE *myFile;
    char sc[path.size() + 1];
    strcpy(sc, path.c_str());
    myFile = fopen(sc, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    while (contador < cantInodos) {
        fwrite("0", sizeof(char), 1, myFile);
        contador++;
    }
    //cerrando stream
    fclose(myFile);
}

INODO *getNewInodo(int type, int permisos, int size) {
    auto end = std::chrono::system_clock::now();
    INODO *nuevo = (INODO *) malloc(sizeof(INODO));
    nuevo->uid = 1;
    nuevo->gid = 1;
    nuevo->size = size;
    nuevo->atime = std::chrono::system_clock::to_time_t(end);
    nuevo->ctime = std::chrono::system_clock::to_time_t(end);
    nuevo->mtime = std::chrono::system_clock::to_time_t(end);
    int i;
    for (i = 0; i < 15; i++) {
        nuevo->block[i] = -1;
    }
    nuevo->type = type;
    nuevo->permisos = permisos;

    return nuevo;
}

void clearArray(char array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

BLOCK_DIR *getNewBlockDir(char name[], int indexDir, char namepad[], int indexPad) {
    BLOCK_DIR *block = (BLOCK_DIR *) malloc(sizeof(BLOCK_DIR));
    int i;
    for (i = 0; i < 4; i++) {
        clearArray(block->content[i].name, 14);
        block->content[i].inodo = -1;
    }
    strcpy(block->content[0].name, name);
    block->content[0].inodo = indexDir;

    strcpy(block->content[1].name, namepad);
    block->content[1].inodo = indexPad;

    return block;
}

int getInitInode(SuperBlock *sb, int index) {
    return sb->inodeStart + (sb->inodeSize * index);
}

int getInitBlock(SuperBlock *sb, int index) {
    return sb->blockStart + (sb->blockSize * index);
}

void writeInodo(INODO *inodo, char path[], int init) {
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(inodo, sizeof(INODO), 1, myFile);
    fclose(myFile);
}

void writeBlockDirectory(BLOCK_DIR *sb, char path[], int init) {
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(sb, sizeof(BLOCK_DIR), 1, myFile);
    //cerrando stream
    fclose(myFile);
}

int getBitmapIndex(int startBm, int finBm, char path[]) {
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return -1;
    }
    char caracter;
    fseek(myFile, startBm, SEEK_SET);
    int contador = 0;
    bool marcado = false;
    while (contador < finBm) {
        fread(&caracter, sizeof(char), 1, myFile);
        if (caracter == '0') {
            if (!marcado) {
                fseek(myFile, -1, SEEK_CUR);
                fwrite("1", sizeof(char), 1, myFile);
                marcado = true;
            } else {
                break;
            }
        }
        contador++;
    }
    fclose(myFile);
    return contador;
}

int writeDirectory(SuperBlock *sb, char path[], char nameDir[], char namePad[], int indexPad) {
    //INODO DE CARPETA NUEVA
    INODO *nuevo = getNewInodo(1, 777, -1);
    int indexI = sb->firstInode;
    //BLOQUE DE CARPETA NUEVA
    BLOCK_DIR *dir = getNewBlockDir(nameDir, indexI, namePad, indexPad);
    //ASIGNANDO BLOQUE A CARPETA
    nuevo->block[0] = sb->firstBlock;
    writeInodo(nuevo, path, getInitInode(sb, indexI));
    writeBlockDirectory(dir, path, getInitBlock(sb, sb->firstBlock));
    delete nuevo;
    delete dir;
    sb->firstInode = getBitmapIndex(sb->bmInodeStart, sb->inodesCount, path);
    sb->firstBlock = getBitmapIndex(sb->bmBlockStart, sb->blocksCount, path);
    sb->freeBlocksCount--;
    sb->freeInodesCount--;
    return indexI;
}

MBR *openMBR(char path[]) {
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error: No existe el disco\n";
        return NULL;
    }
    fseek(myFile, 0, SEEK_SET);
    MBR *mbr = (MBR *) malloc(sizeof(MBR));
    fread(mbr, sizeof(MBR), 1, myFile);
    fclose(myFile);
    return mbr;
}

bool getStartPartition(MBR *disco, char name[], int *init) {
    int i;
    for (i = 0; i < 4; i++) {
        if (disco->partition[i].status == '1') {
            if (strcmp(disco->partition[i].name, name) == 0) {
                *init = disco->partition[i].start;
                return true;
            }
        }
    }
    return false;
}

SuperBlock *readSuperBlock(char path[], char name[]) {
    MBR *disco = openMBR(path);
    if (disco == NULL) {
        std::cout << "Error al leer el disco\n";
        return NULL;
    }
    int init;
    bool res = getStartPartition(disco, name, &init);
    if (res != true) {
        return NULL;
    }
    delete disco;
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return NULL;
    }
    SuperBlock *sb = (SuperBlock *) malloc(sizeof(SuperBlock));

    fseek(myFile, init, SEEK_SET);
    fread(sb, sizeof(SuperBlock), 1, myFile);
    fclose(myFile);
    return sb;
}

SuperBlock *readSuperBlock(char path[], char name[], int *startSb) {
    MBR *disco = openMBR(path);
    if (disco == NULL) {
        std::cout << "Error al leer el disco\n";
        return NULL;
    }
    bool res = getStartPartition(disco, name, startSb);
    if (!res) {
        return NULL;
    }
    delete disco;
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return NULL;
    }
    SuperBlock *sb = (SuperBlock *) malloc(sizeof(SuperBlock));

    fseek(myFile, *startSb, SEEK_SET);
    fread(sb, sizeof(SuperBlock), 1, myFile);
    fclose(myFile);
    return sb;
}

INODO *readInodo(char path[], int init) {
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return NULL;
    }
    INODO *sb = (INODO *) malloc(sizeof(INODO));

    fseek(myFile, init, SEEK_SET);
    fread(sb, sizeof(INODO), 1, myFile);
    fclose(myFile);
    if (sb->ctime == NULL) {
        return NULL;
    }
    return sb;
}

BLOCK_DIR *readBlockDirectory(char path[], int init) {
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return NULL;
    }
    BLOCK_DIR *bd = (BLOCK_DIR *) malloc(sizeof(BLOCK_DIR));

    fseek(myFile, init, SEEK_SET);
    fread(bd, sizeof(BLOCK_DIR), 1, myFile);
    fclose(myFile);
    if (bd->content[0].name[0] == '\0') {
        return NULL;
    }
    int i;
    for (i = 0; i < 4; i++) {
        if (bd->content[i].name[0] == '\0') {
            bd->content[i].inodo = -1;
        }
    }
    return bd;
}

BLOCK_POINTER *readBlockPointer(char path[], int init) {
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return NULL;
    }
    BLOCK_POINTER *bd = (BLOCK_POINTER *) malloc(sizeof(BLOCK_POINTER));

    fseek(myFile, init, SEEK_SET);
    fread(bd, sizeof(BLOCK_POINTER), 1, myFile);
    fclose(myFile);
    return bd;
}

BLOCK_POINTER *getNewBlockPointer() {
    BLOCK_POINTER *bl = (BLOCK_POINTER *) malloc(sizeof(BLOCK_POINTER));
    int j;
    for (j = 0; j < 16; j++) {
        bl->pointers[j] = -1;
    }
    return bl;
}

void writeBlockPointer(BLOCK_POINTER *newPoints, char path[], int init) {
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(newPoints, sizeof(BLOCK_POINTER), 1, myFile);
    //cerrando stream
    fclose(myFile);
}

int createPointersInd(int idPointBlock, int *idBloqueActual, SuperBlock *sb, char path[]) {
    if (idPointBlock - 11 == 1) {
        BLOCK_POINTER *newPoints = getNewBlockPointer();
        int idBloque = sb->firstBlock;
        writeBlockPointer(newPoints, path, getInitBlock(sb, sb->firstBlock));
        sb->firstBlock = getBitmapIndex(sb->bmBlockStart, sb->blocksCount, path);
        sb->freeBlocksCount--;
        *idBloqueActual = idBloque;
        return idBloque;
    } else {
        int res = createPointersInd(idPointBlock - 1, idBloqueActual, sb, path);
        BLOCK_POINTER *pointers = getNewBlockPointer();
        pointers->pointers[0] = res;
        res = sb->firstBlock;
        writeBlockPointer(pointers, path, getInitBlock(sb, sb->firstBlock));
        sb->firstBlock = getBitmapIndex(sb->bmBlockStart, sb->blocksCount, path);
        sb->freeBlocksCount--;
        return res;
    }
    return -1;
}

int getFreeIndexFromBlockPointer(int nivel, INODO *inodo, int indexBloqueActual, char path[], SuperBlock *sb,
                                 int *indexFree) {
    if (inodo->block[nivel] != -1) {
        if (nivel == 12) {
            BLOCK_POINTER *pointers = readBlockPointer(path, getInitBlock(sb, indexBloqueActual));
            if (pointers == NULL) {
                return 0;
            }
            int indexInBlockP;
            for (indexInBlockP = 0; indexInBlockP < 15; indexInBlockP++) {
                if (pointers->pointers[indexInBlockP] == -1) {
                    *indexFree = indexInBlockP;
                    return indexBloqueActual;
                }
            }
        } else {
            BLOCK_POINTER *pointers = readBlockPointer(path, getInitBlock(sb, indexBloqueActual));
            if (pointers == NULL) {
                return 0;
            }
            int indexInBlockP;
            for (indexInBlockP = 0; indexInBlockP < 15; indexInBlockP++) {
                if (pointers->pointers[indexInBlockP] != -1) {
                    int r = getFreeIndexFromBlockPointer(nivel - 1, inodo, pointers->pointers[indexInBlockP], path, sb,
                                                         indexFree);
                    if (r != -1)return r;
                } else {
                    int bloquePadre = indexBloqueActual;
                    int bloque = createPointersInd(nivel - 1, &indexBloqueActual, sb, path);
                    if (bloque == -1) return 0;
                    pointers->pointers[indexInBlockP] = bloque;
                    writeBlockPointer(pointers, path, getInitBlock(sb, bloquePadre));
                    //*indexBloqueActual = bloque;
                    *indexFree = 0;
                    return bloque;
                }
            }
        }
    }
    return -1;
}

//type
//0=file 1=dir 2=pointer
bool getFreeIndexDirectory(char nameDir[], char path[], SuperBlock *sb, int *indexBloqueActual, int *indexInodoActual,
                           int *indexFree, int *type) {
    INODO *inodo = NULL;
    inodo = readInodo(path, getInitInode(sb, *indexInodoActual));
    if (inodo == NULL) {
        return false;
    }
    int idPointBlock = 0;
    bool isDirect = true;
    BLOCK_DIR *dirblock = NULL;
    bool found = false;
    while (idPointBlock < 15 && !found) {
        if (inodo->block[idPointBlock] != -1) {
            if (isDirect) {
                dirblock = readBlockDirectory(path, getInitBlock(sb, inodo->block[idPointBlock]));
                if (strcmp(dirblock->content[0].name, nameDir) == 0) {
                    //BLOQUE ENCONTRADO
                    *indexBloqueActual = inodo->block[idPointBlock];
                    int indexBloque = 2;
                    while (indexBloque < 4) {
                        if (dirblock->content[indexBloque].inodo == -1) {
                            *indexFree = indexBloque;
                            found = true;
                            break;
                        }
                        indexBloque++;
                    }
                }
            } else {
                //LEER APUNTADOR INDIRECTO
                *type = 2;
                *indexBloqueActual = inodo->block[idPointBlock];
                int bloque = getFreeIndexFromBlockPointer(idPointBlock, inodo, *indexBloqueActual, path, sb, indexFree);
                writeInodo(inodo, path, getInitInode(sb, *indexInodoActual));
                if (bloque == -1) {
                    idPointBlock++;
                    continue;
                }
                found = true;
                *indexBloqueActual = bloque;
            }
        } else {
            if (isDirect) {
                //CREAR UN NUEVO BLOQUE
                BLOCK_DIR *nuevo = getNewBlockDir(dirblock->content[0].name, dirblock->content[0].inodo,
                                                  dirblock->content[1].name, dirblock->content[1].inodo);
                inodo->block[idPointBlock] = sb->firstBlock;
                writeBlockDirectory(nuevo, path, getInitBlock(sb, sb->firstBlock));
                writeInodo(inodo, path, getInitInode(sb, *indexInodoActual));
                //restar un bloque y un inodo del super bloque
                *indexBloqueActual = sb->firstBlock;
                *indexFree = 2;
                found = true;
                sb->firstBlock = getBitmapIndex(sb->bmBlockStart, sb->blocksCount, path);
                sb->freeBlocksCount--;
            } else {
                //CREAR NUEVO BLOQUE
                int bloque = createPointersInd(idPointBlock, indexBloqueActual, sb, path);
                if (bloque == -1) return false;
                inodo->block[idPointBlock] = bloque;
                writeInodo(inodo, path, getInitInode(sb, *indexInodoActual));
                //*indexBloqueActual = bloque;
                *indexFree = 0;
                *type = 2;
                found = true;
            }

        }
        idPointBlock++;
        if (idPointBlock > 11) {
            isDirect = false;
        }
    }

    delete inodo;
    delete dirblock;
    return true;
}

BLOCK_FILE *getNewBlockFile() {
    BLOCK_FILE *bl = (BLOCK_FILE *) malloc(sizeof(BLOCK_FILE));
    clearArray(bl->content, 64);;
    return bl;
}

void writeBlockFile(BLOCK_FILE *sb, char path[], int init) {
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(sb, sizeof(BLOCK_FILE), 1, myFile);
    //cerrando stream
    fclose(myFile);
}

void saveBlockFile(BLOCK_FILE *block, SuperBlock *sb, char path[]) {
    writeBlockFile(block, path, getInitBlock(sb, sb->firstBlock));
    sb->firstBlock = getBitmapIndex(sb->blockStart, sb->blocksCount, path);
    sb->freeBlocksCount--;
}

bool addFileBlockPointers(INODO *inodo, int *indexofInodo, BLOCK_FILE *block, SuperBlock *sb, char path[]) {
    if (inodo->block[*indexofInodo] != -1) {
        int free = 0;
        int res = getFreeIndexFromBlockPointer(*indexofInodo, inodo, inodo->block[*indexofInodo], path, sb, &free);
        if (res == -1) {
            return false;
        }
        BLOCK_POINTER *point = readBlockPointer(path, getInitBlock(sb, res));
        point->pointers[free] = sb->firstBlock;
        writeBlockPointer(point, path, getInitBlock(sb, res));
        saveBlockFile(block, sb, path);
    } else {
        //CREAR NUEVO BLOQUE
        int nuevoIndex;
        int bloque = createPointersInd(*indexofInodo, &nuevoIndex, sb, path);
        if (bloque == -1) return false;
        inodo->block[*indexofInodo] = bloque;
        BLOCK_POINTER *point = readBlockPointer(path, getInitBlock(sb, nuevoIndex));
        point->pointers[0] = sb->firstBlock;
        writeBlockPointer(point, path, getInitBlock(sb, nuevoIndex));
        //CREAR BLOQUE CON CONTENIDO
        saveBlockFile(block, sb, path);
    }
    return true;
}

bool
createChildFile(int size, char *text, char path[], char dirPad[], char name[], SuperBlock *sb, int indexBloqueActual,
                int indexInodoPadre) {
    int indexFree = -1;
    //0=file 1=dir 2=pinter
    int type = 1;
    bool res = getFreeIndexDirectory(dirPad, path, sb, &indexBloqueActual, &indexInodoPadre, &indexFree, &type);
    if (res != true) {
        return res;
    }
    INODO *inodo = getNewInodo(0, 664, size);
    //BLOQUE DE ARCHIVO
    BLOCK_FILE *block = getNewBlockFile();
    int indexofInodo = 0;
    int contadorCaracteres = 0;
    int indexCaracteres = 0;
    bool keepSaving = true;
    while (keepSaving) {
        if (contadorCaracteres >= 64 || indexCaracteres >= size) {
            if (indexofInodo < 12) {
                inodo->block[indexofInodo] = sb->firstBlock;
                saveBlockFile(block, sb, path);
                indexofInodo++;
            } else {
                bool r = addFileBlockPointers(inodo, &indexofInodo, block, sb, path);
                if (r == false) {
                    if (indexofInodo < 14) {
                        indexofInodo++;
                        continue;
                    }
                } else if (r != true) {
                    return r;
                }
            }
            block = getNewBlockFile();
            contadorCaracteres = 0;
        }
        block->content[contadorCaracteres] = text[indexCaracteres];
        if (indexCaracteres >= size) {
            break;
        }
        contadorCaracteres++;
        indexCaracteres++;
    }
    writeInodo(inodo, path, getInitInode(sb, sb->firstInode));

    BLOCK_DIR *blockPad = readBlockDirectory(path, getInitBlock(sb, indexBloqueActual));
    blockPad->content[indexFree].inodo = sb->firstInode;
    strcpy(blockPad->content[indexFree].name, name);
    writeBlockDirectory(blockPad, path, getInitBlock(sb, indexBloqueActual));

    sb->firstInode = getBitmapIndex(sb->bmInodeStart, sb->inodesCount, path);
    sb->freeInodesCount--;
    return true;
}

int
findDirectoryInPointers(int level, int indexBlock, int *indexInodoActual, char namedir[], SuperBlock *sb, char path[]) {

    if (level == 1) {
        BLOCK_POINTER *pointers = readBlockPointer(path, getInitBlock(sb, indexBlock));
        int indexofPointer;
        for (indexofPointer = 0; indexofPointer < 14; indexofPointer++) {
            if (pointers->pointers[indexofPointer] != -1) {
                //BUSCANDO EN INODO
                INODO *inodo = readInodo(path, getInitInode(sb, pointers->pointers[indexofPointer]));
                if (inodo == NULL)return -1;
                int indexBlock;
                BLOCK_DIR *block;
                for (indexBlock = 0; indexBlock < 12; indexBlock++) {
                    if (inodo->block[indexBlock] != -1) {
                        if (inodo->type == 1) {
                            block = readBlockDirectory(path, getInitBlock(sb, inodo->block[indexBlock]));
                            if (block == NULL)return -1;
                            int i = 0;
                            if (block->content[i].inodo != -1) {
                                if (strcmp(block->content[i].name, namedir) == 0) {
                                    *indexInodoActual = block->content[0].inodo;
                                    return inodo->block[indexBlock];
                                }
                            }
                        }
                    }
                }
            }
        }

    } else {
        BLOCK_POINTER *pointers = readBlockPointer(path, getInitBlock(sb, indexBlock));
        int indexofPointer;
        for (indexofPointer = 0; indexofPointer < 14; indexofPointer++) {
            if (pointers->pointers[indexofPointer] != -1) {
                int i = findDirectoryInPointers(level - 1, pointers->pointers[indexofPointer], indexInodoActual,
                                                namedir, sb, path);
                if (i != -1) {
                    return i;
                }
            }
        }

    }

    return -1;
}

int findDirectory(char namedir[], char path[], int *indexInodoActual, SuperBlock *sb) {
    INODO *inodo = readInodo(path, getInitInode(sb, *indexInodoActual));
    if (inodo == NULL) {
        return -1;
    }
    int indexBlock;
    BLOCK_DIR *block;
    for (indexBlock = 0; indexBlock < 12; indexBlock++) {
        if (inodo->block[indexBlock] != -1) {
            if (inodo->type == 1) {
                block = readBlockDirectory(path, getInitBlock(sb, inodo->block[indexBlock]));
                if (block == NULL) {
                    return -1;
                }
                int i;
                for (i = 2; i < 4; i++) {
                    if (block->content[i].inodo != -1) {
                        if (strcmp(block->content[i].name, namedir) == 0) {
                            *indexInodoActual = block->content[i].inodo;
                            inodo = readInodo(path, getInitInode(sb, *indexInodoActual));
                            if (inodo == NULL) {
                                return -1;
                            }
                            return inodo->block[0];
                        }
                    }
                }
            }
        }
    }
    for (indexBlock = 12; indexBlock < 14; indexBlock++) {
        int index = findDirectoryInPointers(indexBlock - 11, inodo->block[indexBlock], indexInodoActual, namedir, sb,
                                            path);
        if (index != -1)return index;
    }
    return -1;
}

//type
//0=file 1=dir 2=pointer
bool createChildDirectory(char dirPad[], char dirName[], char path[], SuperBlock *sb, int *indexInodoPadre,
                          int *indexBloqueActual) {
    int indexFree = -1;
    int type = 1;
    bool res = getFreeIndexDirectory(dirPad, path, sb, indexBloqueActual, indexInodoPadre, &indexFree, &type);
    if (res != true) {
        return res;
    }
    if (indexFree == -1) {
        return false;
    }
    int indexnew = writeDirectory(sb, path, dirName, dirPad, *indexInodoPadre);
    if (type == 1) {
        BLOCK_DIR *block = readBlockDirectory(path, getInitBlock(sb, *indexBloqueActual));
        block->content[indexFree].inodo = indexnew;
        strcpy(block->content[indexFree].name, dirName);
        writeBlockDirectory(block, path, getInitBlock(sb, *indexBloqueActual));
        delete block;
    } else if (type == 2) {
        BLOCK_POINTER *block = readBlockPointer(path, getInitBlock(sb, *indexBloqueActual));
        block->pointers[indexFree] = indexnew;
        writeBlockPointer(block, path, getInitBlock(sb, *indexBloqueActual));
        // delete block;
    }
    *indexInodoPadre = indexnew;

    return true;
}


void writeSuperBlock(SuperBlock *sb, char path[], int init) {
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(sb, sizeof(SuperBlock), 1, myFile);
    //cerrando stream
    fclose(myFile);
}

bool createFileWithText(char newPath[], bool createPath, char text[], int size, char path[], char namePartition[]) {
    //VALIDAR QUE HAYA ESPACIO PARA CREAR INODOS Y BLOQUES
    int startSb;
    SuperBlock *sb = readSuperBlock(path, namePartition, &startSb);
    if (sb == NULL) {
        return false;
    }

    int indexInodoPadre = 0;
    std::stringstream ss(newPath);
    std::string token;
    std::string dirPad = "/";
    int indexBloqueActual = 0;
    while (std::getline(ss, token, '/')) {
        if (token != "") {
            //cout<<"padre: "<<dirPad<<endl;
            //cout<<"archivo/carpeta: "<<token<<endl;
            if (ss.tellg() == -1) {
                bool res = createChildFile(size, text, path, &dirPad[0], &token[0], sb, indexBloqueActual,
                                           indexInodoPadre);
                if (res != true)return res;
            } else {
                int indexBloque = findDirectory(&token[0], path, &indexInodoPadre, sb);
                if (indexBloque != -1) {
                    indexBloqueActual = indexBloque;
                } else {
                    if (createPath) {
                        bool rs = createChildDirectory(&dirPad[0], &token[0], path, sb, &indexInodoPadre,
                                                       &indexBloqueActual);;
                        if (rs != true) {
                            return rs;
                        }
                    } else {
                        return false;
                    }
                }
            }
            dirPad = token;
        }
    }
    writeSuperBlock(sb, path, startSb);
    delete sb;
    return true;
}

void addJournal(SuperBlock *sb, int startSb, char path[], Journal *newj) {
    int startOperations = startSb + sizeof(SuperBlock);
    FILE *myFile;
    int contador = 0;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return;
    }
    Journal *journal = (Journal *) malloc(sizeof(Journal));
    fseek(myFile, startOperations, SEEK_SET);
    while (contador < sb->inodesCount) {
        fread(journal, sizeof(Journal), 1, myFile);
        if (journal == NULL) {
            return;
        }
        if (journal->j_operation == EMPTY) {
            fseek(myFile, -sizeof(Journal), SEEK_CUR);
            fwrite(newj, sizeof(Journal), 1, myFile);
            fclose(myFile);
            return;
        }
        contador++;
    }

    fclose(myFile);
}

bool createFile(char newPath[], bool createPath, char pathFile[], bool isRecovery) {

    char *ide = active_sesion->id;
    char id[10000];
    strcpy(id, ide);

    string pathDisk = getPathDisk(id);
    string partition = getNamePartition(id);

    char path[pathDisk.size() + 1];
    strcpy(path, pathDisk.c_str());
    char namePartition[partition.size() + 1];
    strcpy(namePartition, partition.c_str());

    FILE *fileText;
    fileText = fopen(pathFile, "r+");
    if (fileText == NULL) {
        cout << "Error al abrir el archivo en ruta \"" << pathFile << "\"\n";
        return false;
    }
    fseek(fileText, 0L, SEEK_END);
    int file_size = ftell(fileText);

    char txt[file_size];
    fseek(fileText, 0L, SEEK_SET);
    fread(txt, file_size, 1, fileText);

    fclose(fileText);
    auto end = std::chrono::system_clock::now();
    bool res = createFileWithText(newPath, createPath, txt, file_size, path, namePartition);
    if (res == true && !isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = MKFILE_PATH;
        newj->j_path = newPath;
//        newj->j_group = active_sesion->idGrp;
//        newj->j_user = active_sesion->idUser;
        newj->j_content = pathFile;
        newj->j_boolean = createPath;
        newj->j_perms = 664;
        newj->j_date = std::chrono::system_clock::to_time_t(end);

        int startSb;
        SuperBlock *sb = readSuperBlock(path, namePartition, &startSb);
        if (sb == NULL) {
            return false;
        }
        addJournal(sb, startSb, path, newj);
        delete sb;
    }
    return res;
}

struct ParamsCreateFile{
    char *path;
    bool r = false;
    int size;
    char *contenido;
};

ParamsCreateFile CreateFilesSeparar(string params[4]){
    ParamsCreateFile paramsReturn;
    for (int i = 0; i < 4; ++i) {
        if(params[i][0] == '0' || params[i][0] == '1' || params[i][0] == '2' || params[i][0] == '3' ||
           params[i][0] == '4' || params[i][0] == '5' || params[i][0] == '6' || params[i][0] == '7' ||
           params[i][0] == '8' || params[i][0] == '9'){
            paramsReturn.size = stoi(params[i]);
        }else if (params[i][0] == '#'){
            cin>>paramsReturn.contenido;
        }else if (params[i][0] == '/'){
            strcpy(paramsReturn.path , params[i].c_str());
        }else if (params[i] == "re"){
            paramsReturn.r = true;
        }

    }
    return paramsReturn;
}


bool createFile(char newPath[], bool createPath, int size, bool isRecovery) {
    char *ide = active_sesion->id;
    char id[10000];
    strcpy(id, ide);

    string pathDisk = getPathDisk(id);
    string partition = getNamePartition(id);

    char path[pathDisk.size() + 1];
    strcpy(path, pathDisk.c_str());
    char namePartition[partition.size() + 1];
    strcpy(namePartition, partition.c_str());

    //generar texto
    char *txt = (char *) malloc(sizeof(char) * size);
    int i;
    char caracter = '0';
    for (i = 0; i < size; i++) {
        txt[i] = caracter;
        caracter++;
        if (caracter > '9') {
            caracter = '0';
        }
    }
    bool res = createFileWithText(newPath, createPath, txt, size, path, namePartition);
    auto end = std::chrono::system_clock::now();
    if (res == true && !isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = MKFILE_SIZE;
        newj->j_path = newPath;
//        newj->j_group = active_sesion->idGrp;
//        newj->j_user = active_sesion->idUser;
        newj->j_size = size;
        newj->j_perms = 664;
        newj->j_boolean = createPath;
        newj->j_date = std::chrono::system_clock::to_time_t(end);

        int startSb;
        SuperBlock *sb = readSuperBlock(path, namePartition, &startSb);
        if (sb == NULL) {
            return false;
        }
        addJournal(sb, startSb, path, newj);
        delete sb;
    }
    return res;
}

void fillJournal(int init, int cant, char path[]) {
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return;
    }
    Journal *journal = new Journal();
    fseek(myFile, init, SEEK_SET);
    int contador = 0;
    while (contador < cant) {
        fwrite(journal, sizeof(Journal), 1, myFile);
        contador++;
    }
    fclose(myFile);
}

bool createDirectory(bool createMk, char id[], char path[], bool isRecovery) {
    string pathDisk = getPathDisk(id);
    string namePartition = getNamePartition(id);

    char pathRead[pathDisk.size() + 1];
    strcpy(pathRead, pathDisk.c_str());
    char nameRead[namePartition.size() + 1];
    strcpy(nameRead, namePartition.c_str());

    //VALIDAR QUE HAYA ESPACIO PARA CREAR INODOS Y BLOQUES
    int startSb;
    SuperBlock *sb = readSuperBlock(pathRead, nameRead, &startSb);
    if (sb == NULL) {
        return false;
    }
    int indexInodoPadre = 0;
    int indexBloqueActual = 0;
    std::stringstream ss(path);
    std::string token;
    std::string dirPad = "/";
    while (std::getline(ss, token, '/')) {
        if (token != "") {
            //cout<<"padre: "<<dirPad<<endl;
            //cout<<"carpeta: "<<token<<endl;
            if (ss.tellg() == -1) {
                bool res = createChildDirectory(&dirPad[0], &token[0], pathRead, sb, &indexInodoPadre,
                                                &indexBloqueActual);
                if (res != true)return res;
            } else {
                int indexBloque = findDirectory(&token[0], pathRead, &indexInodoPadre, sb);
                if (indexBloque != -1) {
                    indexBloqueActual = indexBloque;
                } else {
                    if (createMk) {
                        bool res = createChildDirectory(&dirPad[0], &token[0], pathRead, sb, &indexInodoPadre,
                                                        &indexBloqueActual);;
                        if (res != true) {
                            return res;
                        }
                    } else {
                        return false;
                    }
                }

            }
            dirPad = token;
        }
    }
    writeSuperBlock(sb, pathRead, startSb);
    auto end = std::chrono::system_clock::now();
    if (!isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = MKDIRECTORY;
        newj->j_path = path;
        newj->j_boolean = createMk;
//        newj->j_group = active_sesion->idGrp;
//        newj->j_user = active_sesion->idUser;
        newj->j_perms = 777;
        newj->j_date = std::chrono::system_clock::to_time_t(end);
        addJournal(sb, startSb, pathRead, newj);
    }
    return true;
}

BLOCK_FILE *readBlockFile(char path[], int init) {
    FILE *myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco \n";
        return NULL;
    }
    BLOCK_FILE *bd = (BLOCK_FILE *) malloc(sizeof(BLOCK_FILE));
    fseek(myFile, init, SEEK_SET);
    fread(bd, sizeof(BLOCK_FILE), 1, myFile);
    fclose(myFile);
    return bd;
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


    cout << "se formateo la particion" << endl;
    writeBitmap(SBnuevo.inodesCount, SBnuevo.bmInodeStart, getPathDisk(id));
    writeBitmap(SBnuevo.blocksCount, SBnuevo.bmBlockStart, getPathDisk(id));
    char pathdir[path.size() + 1];
    strcpy(pathdir, path.c_str());
    writeDirectory(&SBnuevo, pathdir, "/", "/", 0);
    writeDirectory(&SBnuevo, pathdir, "/root", "/", 0);
    fseek(file, start, SEEK_SET);
    fwrite(&SBnuevo, sizeof(SuperBlock), 1, file);
    fclose(file);

    char *users = "1,G,root\n1,U,root,root,123\n";
    char namecrt[name.size() + 1];
    strcpy(namecrt, name.c_str());
    createFileWithText("/users.txt", true, users, 28, pathdir, namecrt);
    cout << "se creo la carpeta raiz" << endl;
    SBnuevo.freeBlocksCount = SBnuevo.freeBlocksCount - 1;
    SBnuevo.freeInodesCount = SBnuevo.freeInodesCount - 1;
    SBnuevo.firstInode = 1;
    SBnuevo.firstBlock = 1;
    SBnuevo.inodesCount = 1;
    SBnuevo.blocksCount = 1;
}

struct mkdirParams {
    string path;
    string id;
};

void mkdirMethod(mkdirParams params) {
    string pathDisk = getPathDisk(params.id);
    string namePartition = getNamePartition(params.id);

    char pathRead[pathDisk.size() + 1];
    strcpy(pathRead, pathDisk.c_str());
    char nameRead[namePartition.size() + 1];
    strcpy(nameRead, namePartition.c_str());
    SuperBlock *sb = readSuperBlock(pathRead, nameRead);
    SuperBlock superbloque;
    if (sb == NULL) {
        return;
    }
}

mkdirParams mkdirSepParams(string params[3]) {
    mkdirParams parametros;
    for (int i = 0; i < 3; ++i) {
        if (params[i][0] == '/') {
            parametros.path = params[i];
        } else if (params[i][0] == '8') {
            parametros.id = params[i];
        }
    }
    return parametros;
}

std::string getContentFileFromPointers(int level, int indexBlock, char path[], SuperBlock *sb) {
    BLOCK_POINTER *points = readBlockPointer(path, getInitBlock(sb, indexBlock));
    if (points == NULL) {
        return "";
    }
    if (level == 1) {
        string contenido = "";
        int indexBloque;
        for (indexBloque = 0; indexBloque < 15; indexBloque++) {
            if (points->pointers[indexBloque] != -1) {
                BLOCK_FILE *file = readBlockFile(path, getInitBlock(sb, points->pointers[indexBloque]));
                if (file == NULL) {
                    return "";
                }
                int index;
                for (index = 0; index < 64; index++) {
                    contenido += file->content[index];
                }

            }
        }
        return contenido;
    } else {
        string contenido = "";
        int indexBloque;
        for (indexBloque = 0; indexBloque < 15; indexBloque++) {
            if (points->pointers[indexBloque] != -1) {
                contenido += getContentFileFromPointers(level - 1, points->pointers[indexBloque], path, sb);
            }
        }
        return contenido;
    }
}

std::string getContentFile(int indexInodo, char path[], SuperBlock *sb) {
    string contenido = "";
    INODO *inodo = readInodo(path, getInitInode(sb, indexInodo));
    if (inodo == NULL) {
        return "";
    }
    if (inodo->type == 0) {
        int indexBloque;
        for (indexBloque = 0; indexBloque < 12; indexBloque++) {
            if (inodo->block[indexBloque] != -1) {
                BLOCK_FILE *file = readBlockFile(path, getInitBlock(sb, inodo->block[indexBloque]));
                if (file == NULL) {
                    return "";
                }
                int index;
                for (index = 0; index < 64; index++) {
                    contenido += file->content[index];
                }

            }
        }
        //APUNTADORES INDIRECTOS
        for (indexBloque = 12; indexBloque < 14; indexBloque++) {
            if (inodo->block[indexBloque] != -1) {
                contenido += getContentFileFromPointers(indexBloque - 11, inodo->block[indexBloque], path, sb);
            }
        }
    }
    return contenido;
}

int findFile(char filePath[], char path[], char partition[], char **title) {
    //VALIDAR QUE HAYA ESPACIO PARA CREAR INODOS Y BLOQUES
    int startSb;
    SuperBlock *sb = readSuperBlock(path, partition, &startSb);
    if (sb == NULL) {
        return false;
    }

    int indexInodoPadre = 0;
    std::stringstream ss(filePath);
    std::string token;
    std::string dirPad = "/";
    int indexBloqueActual = 0;
    while (std::getline(ss, token, '/')) {
        if (token != "") {
            //cout<<"padre: "<<dirPad<<endl;
            //cout<<"carpeta: "<<token<<endl;
            if (ss.tellg() == -1) {
                //inodo de directorio
                INODO *inodo = readInodo(path, getInitInode(sb, indexInodoPadre));
                if (inodo == NULL) {
                    return false;
                }
                if (inodo->type == 1) {
                    int indexBloqueInodo;
                    for (indexBloqueInodo = 0; indexBloqueInodo < 12; indexBloqueInodo++) {
                        if (inodo->block[indexBloqueInodo] != -1) {
                            BLOCK_DIR *directory = readBlockDirectory(path,
                                                                      getInitBlock(sb, inodo->block[indexBloqueInodo]));
                            if (directory == NULL) {
                                return false;
                            }
                            int indexInodoBloque;
                            for (indexInodoBloque = 2; indexInodoBloque < 4; indexInodoBloque++) {
                                if (strcmp(directory->content[indexInodoBloque].name, &token[0]) == 0) {
                                    //encontrado
                                    INODO *file = readInodo(path, getInitInode(sb,
                                                                               directory->content[indexInodoBloque].inodo));
                                    if (file != NULL) {
                                        if (file->type == 0) {
                                            *title = (char *) malloc(sizeof(directory->content[indexInodoBloque].name));
                                            strcpy(*title, directory->content[indexInodoBloque].name);
                                            return directory->content[indexInodoBloque].inodo;
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    return -1;
                }
            } else {
                int indexBloque = findDirectory(&token[0], path, &indexInodoPadre, sb);
                if (indexBloque != -1) {
                    indexBloqueActual = indexBloque;
                } else {
                    cout << "ocurrio un error buscando el archivo" << endl;
                    return -1;
                }
                dirPad = token;
            }
        }
    }
    return -1;
}

string findContentFile(char filePath[], char path[], char partition[], char **title) {
    int indexInode = findFile(filePath, path, partition, title);
    if (indexInode == -1) {
        return "";
    }
    int startSb = -1;
    SuperBlock *sb = readSuperBlock(path, partition, &startSb);
    if (sb == NULL) {

        return "";
    }
    string contentResponse = getContentFile(indexInode, path, sb);
    delete sb;
    return contentResponse;
}

bool catFile(char filePath[], char path[], char partition[]) {
    char *title;
    char pathRes[10000];
    char nameRes[10000];
    strcpy(pathRes, path);
    strcpy(nameRes, partition);
    string res = findContentFile(filePath, pathRes, nameRes, &title);
    //cout<<"--->"<<title<<"<---\n";
    cout << res << endl;
    return true;
}

Group *getGroup(char name[], char *contentUsers) {
    int contadortoken;
    string nameG(name);
    Group *grp = NULL;
    std::stringstream ss(contentUsers);
    std::string token;

    while (std::getline(ss, token, '\n')) {
        grp = new Group();
        contadortoken = 0;
        std::stringstream line(token);
        std::string tokenLine;
        while (std::getline(line, tokenLine, ',')) {
            if (contadortoken == 0) {
                if (tokenLine == "0") {
                    continue;
                } else {
                    grp->id = tokenLine;
                }
            } else if (contadortoken == 1) {
                if (tokenLine != "G") {
                    grp = NULL;
                    break;
                }
            } else if (contadortoken == 2) {
                if (tokenLine == nameG) {
                    grp->name = tokenLine;
                } else {
                    grp = NULL;
                    break;
                }
            } else {
                break;
            }
            contadortoken++;
        }
        if (grp != NULL)return grp;
    }
    return grp;
}

User *getUser(char usr[], char *contentUsers) {
    int contadortoken;
    string nameUser(usr);
    User *user = NULL;
    std::stringstream ss(contentUsers);
    std::string token;

    while (std::getline(ss, token, '\n')) {
        if (token != "") {
            user = new User();
            contadortoken = 0;
            std::stringstream line(token);
            std::string tokenLine;
            while (std::getline(line, tokenLine, ',')) {
                if (tokenLine != "") {
                    if (contadortoken == 0) {
                        if (tokenLine == "0") {
                            continue;
                        } else {
                            user->id = tokenLine;
                        }
                    } else if (contadortoken == 1) {
                        if (tokenLine != "U") {
                            user = NULL;
                            break;
                        }
                    } else if (contadortoken == 2) {
                        user->group = tokenLine;
                    } else if (contadortoken == 3) {
                        if (tokenLine == nameUser) {
                            user->name = tokenLine;
                        } else {
                            user = NULL;
                            break;
                        }
                    } else if (contadortoken == 4) {
                        user->pwd = tokenLine;
                    } else {
                        break;
                    }
                    contadortoken++;
                }
            }
            if (user != NULL) {
                Group *grp = getGroup(&user->group[0], contentUsers);
                if (grp != NULL) {
                    user->group = grp->id;
                }
                return user;
            }
        }
    }

    if (user != NULL) {
        Group *grp = getGroup(&user->group[0], contentUsers);
        if (grp != NULL) {
            user->group = grp->id;
        }
    }

    return user;
}

int countUsers(char contentUsers[]) {
    int contadortoken;
    std::stringstream ss(contentUsers);
    std::string token;
    int contadorUsuarios = 0;
    while (std::getline(ss, token, '\n')) {
        if (token != "") {
            contadortoken = 0;
            std::stringstream line(token);
            std::string tokenLine;
            while (std::getline(line, tokenLine, ',')) {
                if (contadortoken == 0) {
                    if (tokenLine == "0") {
                        continue;
                    }
                } else if (contadortoken == 1) {
                    if (tokenLine == "U") {
                        contadorUsuarios++;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
                contadortoken++;
            }
        }
    }
    return contadorUsuarios;
}

int countGroups(char contentGroups[]) {
    int contadortoken;
    std::stringstream ss(contentGroups);
    std::string token;
    int contadorGrupos = 0;
    while (std::getline(ss, token, '\n')) {
        if (token != "") {
            contadortoken = 0;
            std::stringstream line(token);
            std::string tokenLine;
            while (std::getline(line, tokenLine, ',')) {
                if (contadortoken == 0) {
                    if (tokenLine == "0") {
                        continue;
                    }
                } else if (contadortoken == 1) {
                    if (tokenLine == "G") {
                        contadorGrupos++;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
                contadortoken++;
            }
        }
    }
    return contadorGrupos;
}

bool ReplaceContentFile(int indexInode, char *content, char path[], char namePart[]) {
    int startSb = 0;
    SuperBlock *sb = readSuperBlock(path, namePart, &startSb);
    if (sb == NULL) {
        return false;
    }
    INODO *inodo = readInodo(path, getInitInode(sb, indexInode));
    if (inodo == NULL) {
        return false;
    }
    BLOCK_FILE *block = getNewBlockFile();
    int indexofInodo = 0;
    int contadorCaracteres = 0;
    int indexCaracteres = 0;
    bool keepSaving = true;
    while (keepSaving) {
        if (contadorCaracteres >= 64 || content[indexCaracteres] == '\0') {
            if (indexofInodo < 12) {
                if (inodo->block[indexofInodo] != -1) {
                    writeBlockFile(block, path, getInitBlock(sb, inodo->block[indexofInodo]));
                    indexofInodo++;
                } else {
                    inodo->block[indexofInodo] = sb->firstBlock;
                    saveBlockFile(block, sb, path);
                    indexofInodo++;
                }
            } else {
                /*Response r = addFileBlockPointers(inodo,&indexofInodo,block,sb,path);
                if(r==ERROR_LEVEL_FULL){
                    if(indexofInodo<14){
                         indexofInodo++;
                         continue;
                    }
                }else if(r!=SUCCESS){
                    return r;
                }*/
            }
            block = getNewBlockFile();
            contadorCaracteres = 0;
        }
        block->content[contadorCaracteres] = content[indexCaracteres];
        if (content[indexCaracteres] == '\0') {
            break;
        }
        contadorCaracteres++;
        indexCaracteres++;
    }
    writeInodo(inodo, path, getInitInode(sb, indexInode));
    return true;
}

struct UsrParams {
    char *user;
    char *pass;
    char *group;
};

UsrParams separarUsrParams(string datos[3]){
    UsrParams parametrosRetorno;
    string params[3];
    string retorno[3];
//    char auxi[100] ="";
//    char auxi1[100]="";
//    char auxi2[100]="";
    for (int i = 0; i < 3; ++i) {
        params[i] = datos[i];
        if (params[i][0] == '%') {
            retorno[0]=params[i];
//            strcpy(parametrosRetorno.group, params[i].c_str());
        } else if (params[i][0] == '#') {
            retorno[1]=params[i];
//            string param = params[i].substr(1, params[i].length() - 1);
//            strcpy(parametrosRetorno.user, param.c_str());
        } else {
            retorno[2]=params[i];
//            strcpy(parametrosRetorno.pass, params[i].c_str());
        }
    }
//    string param1 = retorno[0].substr(1, retorno[0].length() - 1);
//    strcpy(auxi,param1.c_str());
//    parametrosRetorno.group =auxi;
//    string parami = retorno[1].substr(1, retorno[1].length() - 1);
//    strcpy(auxi1,parami.c_str());
//    parametrosRetorno.user =auxi1;
//    string parami2 = retorno[2].substr(0, retorno[2].length());
//    strcpy(auxi2,parami2.c_str());
//    parametrosRetorno.pass =auxi2;
    return parametrosRetorno;
}

bool addUser(char ide[], char usr[], char pwd[], char grp[], bool isRecovery) {
    char id[10000];
    strcpy(id, ide);

    string pathDisk = getPathDisk(id);
    string namePartition = getNamePartition(id);

    char path[pathDisk.size() + 1];
    strcpy(path, pathDisk.c_str());
    char partition[namePartition.size() + 1];
    strcpy(partition, namePartition.c_str());

    char *title;
    char *filePath = "/users.txt";
    //******************
    int indexInode = findFile(filePath, path, partition, &title);
    if (indexInode == -1) {
        return false;
    }
    int startSb = -1;
    SuperBlock *sb = readSuperBlock(path, partition, &startSb);
    if (sb == NULL) {
        return false;
    }

    string resContent = getContentFile(indexInode, path, sb);
    char *content = &resContent[0];
    //***************************
    Group *grpp = getGroup(grp, content);
    if (grpp == NULL) {
        return false;
    }
    int contadorUsuarios = countUsers(content);
    User *user = getUser(usr, content);
    if (user != NULL) {
        return false;
    }

    string newContent = "";
    int contador = 0;
    while (resContent[contador] != '\0') {
        newContent += resContent[contador];
        contador++;
    }
    newContent += to_string(contadorUsuarios + 1);
    newContent += ",U,";
    newContent += grp;
    newContent += ",";
    newContent += usr;
    newContent += ",";
    newContent += pwd;
    newContent += "\n";
    bool r = ReplaceContentFile(indexInode, &newContent[0], path, partition);
    auto end = std::chrono::system_clock::now();
    if (r == true && !isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = ADDUSER;
        newj->j_user = usr;
        newj->j_group = grp;
        newj->j_content = pwd;
        newj->j_date = std::chrono::system_clock::to_time_t(end);
        addJournal(sb, startSb, path, newj);
    }
    delete sb;
    cout<<"creacion de usuario exitosa"<<endl;
    return r;
}

bool addGroup(char ide[], char grp[], bool isRecovery) {
    char *title;
    char *filePath = "/users.txt";
    char id[10000];
    strcpy(id, ide);

    string pathDisk = getPathDisk(id);
    string namePartition = getNamePartition(id);

    char path[pathDisk.size() + 1];
    strcpy(path, pathDisk.c_str());
    char partition[namePartition.size() + 1];
    strcpy(partition, namePartition.c_str());

    //******************
    int indexInode = findFile(filePath, path, partition, &title);
    if (indexInode == -1) {
        return false;
    }
    int startSb = -1;
    SuperBlock *sb = readSuperBlock(path, partition, &startSb);
    if (sb == NULL) {
        return false;
    }
    std::string resContent = getContentFile(indexInode, path, sb);
    char *content = &resContent[0];
    //***************************
    int contadorGrupos = countGroups(content);
    Group *grpp = getGroup(grp, content);
    if (grpp != NULL) {
        return false;
    }

    string newContent = "";
    int contador = 0;
    while (resContent[contador] != '\0') {
        newContent += resContent[contador];
        contador++;
    }
    newContent += to_string(contadorGrupos + 1);
    newContent += ",G,";
    newContent += grp;
    newContent += "\n";
    bool r = ReplaceContentFile(indexInode, &newContent[0], path, partition);
    auto end = std::chrono::system_clock::now();
    if (r == false && !isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = ADDGRP;
        newj->j_group = grp;
        newj->j_date = std::chrono::system_clock::to_time_t(end);
        addJournal(sb, startSb, path, newj);
    }
    delete sb;
    return r;
}

User *getUser(char usr[], char path[], char namePartition[]) {
    char *title;
    char *filePath = "/users.txt";
    char user[10000];
    strcpy(user, usr);
    string res = findContentFile(filePath, path, namePartition, &title);
    return getUser(user, &res[0]);
}

User *getUserById(char id[], char path[], char namePartition[]) {
    char *title;
    char *filePath = "/users.txt";
    string res = findContentFile(filePath, path, namePartition, &title);
    //**********************
    int contadortoken;
    string idUser(id);
    User *user = NULL;
    std::stringstream ss(res);
    std::string token;
    bool found = false;
    while (std::getline(ss, token, '\n')) {
        if (token != "") {
            user = new User();
            contadortoken = 0;
            std::stringstream line(token);
            std::string tokenLine;
            while (std::getline(line, tokenLine, ',')) {
                if (tokenLine != "") {
                    if (contadortoken == 0) {
                        if (tokenLine == "0") {
                            continue;
                        } else {
                            user->id = tokenLine;
                            if (tokenLine == idUser) {
                                found = true;
                            }
                        }
                    } else if (contadortoken == 1) {
                        if (tokenLine != "U") {
                            user = NULL;
                            break;
                        }
                    } else if (contadortoken == 2) {
                        user->group = tokenLine;
                    } else if (contadortoken == 3) {
                        user->name = tokenLine;
                    } else if (contadortoken == 4) {
                        user->pwd = tokenLine;
                    } else {
                        break;
                    }
                    contadortoken++;
                }
            }
            if (user != NULL && found) {
                return user;
            }
        }
    }
    if (!found) return NULL;
    return user;
}

Group *getGroupById(char name[], char path[], char namePartition[]) {
    char *title;
    char *filePath = "/users.txt";
    string res = findContentFile(filePath, path, namePartition, &title);
    //**********************
    int contadortoken;
    string nameG(name);
    Group *grp = NULL;
    std::stringstream ss(res);
    std::string token;
    bool found = false;
    while (std::getline(ss, token, '\n')) {
        grp = new Group();
        contadortoken = 0;
        std::stringstream line(token);
        std::string tokenLine;
        while (std::getline(line, tokenLine, ',')) {
            if (contadortoken == 0) {
                if (tokenLine == "0") {
                    continue;
                } else {
                    grp->id = tokenLine;
                    if (tokenLine == nameG) {
                        found = true;
                    }
                }
            } else if (contadortoken == 1) {
                if (tokenLine != "G") {
                    grp = NULL;
                    break;
                }
            } else if (contadortoken == 2) {
                grp->name = tokenLine;
            } else {
                break;
            }
            contadortoken++;
        }
        if (grp != NULL && found) {
            return grp;
        }
    }
    if (!found) return NULL;
    return grp;
}

void clearInodePointers(INODO *inodo) {
    if (inodo != NULL) {
        int i;
        for (i = 0; i < 15; i++) {
            inodo->block[i] = -1;
        }
    }
}

bool editFile(char pathFile[], char newCont[], bool isRecovery) {

    char *ide = active_sesion->id;
    char id[10000];
    strcpy(id, ide);

    string pathDisk = getPathDisk(id);
    string partition = getNamePartition(id);

    char path[pathDisk.size() + 1];
    strcpy(path, pathDisk.c_str());
    char namePart[partition.size() + 1];
    strcpy(namePart, partition.c_str());

    char *title;
    int indexInode = findFile(pathFile, path, namePart, &title);
    if (indexInode == -1) {
        return false;
    }
    int startSb = -1;
    SuperBlock *sb = readSuperBlock(path, namePart, &startSb);
    if (sb == NULL) {
        return false;
    }
    string content = getContentFile(indexInode, path, sb);
    string newContent = "";
    int contador = 0;
    while (content[contador] != '\0') {
        newContent += content[contador];
        contador++;
    }
    newContent += newCont;
    bool r = ReplaceContentFile(indexInode, &newContent[0], path, namePart);
    auto end = std::chrono::system_clock::now();
    if (r == true && !isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = EDIT_FILE;
//        newj->j_user =active_sesion->idUser;
//        newj->j_group = active_sesion->idGrp;
        newj->j_content = newCont;
        newj->j_path = pathFile;
        newj->j_date = std::chrono::system_clock::to_time_t(end);
        addJournal(sb, startSb, path, newj);
    }
    delete sb;
    return r;
}

bool deleteUser(char ide[], char name[], bool isRecovery) {
    char id[10000];
    strcpy(id, ide);

    char *title;
    char *filePath = "/users.txt";
    string pathDisk = getPathDisk(id);
    string namePartition = getNamePartition(id);


    char path[pathDisk.size() + 1];
    strcpy(path, pathDisk.c_str());
    char partition[namePartition.size() + 1];
    strcpy(partition, namePartition.c_str());
    int indexInode = findFile(filePath, path, partition, &title);
    if (indexInode == -1) {
        return false;
    }
    int startSb = -1;
    SuperBlock *sb = readSuperBlock(path, partition, &startSb);
    if (sb == NULL) {
        return false;
    }
    string content = getContentFile(indexInode, path, sb);
    //if(content!=NULL)return ERROR_UNHANDLED;
    //***********************************************************
    std::stringstream ss(content);
    std::string token;
    std::string newContent = "";
    int contadorToken = 0;
    bool found = false;
    bool save = false;
    while (std::getline(ss, token, '\n')) {
        if (!found) {
            //******************************
            std::stringstream ss2(token);
            std::string tokenLine;
            std::string newline = "";
            contadorToken = 0;
            while (std::getline(ss2, tokenLine, ',')) {
                if (token != "") {
                    if (contadorToken == 0) {
                        if (tokenLine != "0") {
                            newline += "0,";
                        } else {
                            newline += tokenLine;
                            newline += ",";
                        }
                    } else if (contadorToken == 1) {
                        if (tokenLine != "U") {
                            break;
                        } else {
                            newline += "U,";
                        }
                    } else if (contadorToken == 2) {
                        newline += tokenLine;
                        newline += ",";
                        if (name == tokenLine) {
                            save = true;
                            found = true;
                        } else {
                            break;
                        }
                    } else if (contadorToken == 3) {
                        newline += tokenLine;
                        newline += ",";
                    } else {
                        newline += tokenLine;
                        newline += "\n";
                    }
                }
                contadorToken++;
            }
            if (save) {
                newContent += newline;
                newContent += "\n";
            } else {
                newContent += token;
                newContent += "\n";
            }
            //******************************
        } else {
            newContent += token;
            newContent += "\n";
        }
    }
    char *my_argument = const_cast<char *> (newContent.c_str());
    bool r = ReplaceContentFile(indexInode, my_argument, path, partition);
    auto end = std::chrono::system_clock::now();
    if (r == true && !isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = DELUSER;
        newj->j_user = name;
        newj->j_date = std::chrono::system_clock::to_time_t(end);
        addJournal(sb, startSb, path, newj);
    }
    delete sb;
    return r;
}

bool deleteGroup(char ide[], char name[], bool isRecovery) {
    char *title;
    char *filePath = "/users.txt";

    char id[10000];
    strcpy(id, ide);

    string pathDisk = getPathDisk(id);
    string namePartition = getNamePartition(id);

    char path[pathDisk.size() + 1];
    strcpy(path, pathDisk.c_str());
    char partition[namePartition.size() + 1];
    strcpy(partition, namePartition.c_str());

    int indexInode = findFile(filePath, path, partition, &title);
    if (indexInode == -1) {
        return false;
    }
    int startSb = -1;
    SuperBlock *sb = readSuperBlock(path, partition, &startSb);
    if (sb == NULL) {
        return false;
    }
    std::string content = getContentFile(indexInode, path, sb);
    //***********************************************************
    std::stringstream ss(content);
    std::string token;
    std::string newContent = "";
    int contadorToken = 0;
    bool found = false;
    bool save = false;
    while (std::getline(ss, token, '\n')) {
        if (!found) {
            //******************************
            std::stringstream ss2(token);
            std::string tokenLine;
            std::string newline = "";
            contadorToken = 0;
            while (std::getline(ss2, tokenLine, ',')) {
                if (token != "") {
                    if (contadorToken == 0) {
                        if (tokenLine != "0") {
                            newline += "0,";
                        } else {
                            newline += tokenLine;
                            newline += ",";
                        }
                    } else if (contadorToken == 1) {
                        if (tokenLine != "G") {
                            break;
                        } else {
                            newline += "G,";
                        }
                    } else if (contadorToken == 2) {
                        newline += tokenLine;
                        newline += "\n";
                        if (name == tokenLine) {
                            save = true;
                            found = true;
                        } else {
                            break;
                        }
                    }
                }
                contadorToken++;
            }
            if (save) {
                newContent += newline;
                newContent += "\n";
            } else {
                newContent += token;
                newContent += "\n";
            }
            //******************************
        } else {
            newContent += token;
            newContent += "\n";
        }
    }
    char *my_argument = const_cast<char *> (newContent.c_str());
    bool r = ReplaceContentFile(indexInode, my_argument, path, partition);
    auto end = std::chrono::system_clock::now();
    if (r == false && !isRecovery) {
        //AGREGAR A JOURNAL
        Journal *newj = new Journal();
        newj->j_operation = DELGRP;
        newj->j_group = name;
        newj->j_date = std::chrono::system_clock::to_time_t(end);
        addJournal(sb, startSb, path, newj);
    }
    delete sb;
    return r;
}

bool clearAllSystem(char path[], char name[]) {
    int startSb;
    SuperBlock *sb = readSuperBlock(path, name, &startSb);
    if (sb == NULL) {
        return false;
    }
    if (sb->filesystemType == 2) {
        return false;
    }
    //limpiando bitmaps
    int contador;
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return false;
    }
    //LIMPIAR BITMAP DE INODOS
    fseek(myFile, sb->bmInodeStart, SEEK_SET);
    contador = 0;
    while (contador < sb->inodesCount) {
        fwrite("\0", sizeof(char), 1, myFile);
        contador++;
    }
    //LIMPIAR BITMAP DE BLOQUES
    fseek(myFile, sb->bmBlockStart, SEEK_SET);
    contador = 0;
    while (contador < sb->blocksCount) {
        fwrite("\0", sizeof(char), 1, myFile);
        contador++;
    }
    //LIMPIAR BITMAP DE INODOS
    fseek(myFile, sb->bmInodeStart, SEEK_SET);
    contador = 0;
    while (contador < sb->blocksCount) {
        fwrite("\0", sizeof(char), 1, myFile);
        contador++;
    }
    //LIMPIAR INODOS
    fseek(myFile, sb->inodeStart, SEEK_SET);
    contador = 0;
    while (contador < (sb->inodesCount * sb->inodeSize)) {
        fwrite("\0", sizeof(char), 1, myFile);
        contador++;
    }
    //LIMPIAR BLOQUES
    fseek(myFile, sb->blockStart, SEEK_SET);
    contador = 0;
    while (contador < (sb->blocksCount * sb->blockSize)) {
        fwrite("\0", sizeof(char), 1, myFile);
        contador++;
    }
    //cerrando stream
    fclose(myFile);
    return true;
}

bool recoverySystem(SuperBlock *sb, int startSb, char path[], char namePartition[], char id[]) {
    int startOperations = startSb + sizeof(SuperBlock);
    ParamsMKFS parametros;
    parametros.fs = 3;
    parametros.type = "full";
    parametros.id = id;
    mkfsMethod(parametros);
    FILE *myFile;
    int contador = 0;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return false;
    }
    Journal *journal = (Journal *) malloc(sizeof(Journal));
    fseek(myFile, startOperations, SEEK_SET);
    while (contador < sb->inodesCount) {
        fread(journal, sizeof(Journal), 1, myFile);
        if (journal == NULL) {
            return false;
        }
        if (journal->j_operation == EMPTY) {
            return true;
        }
        switch (journal->j_operation) {
            case MKDIRECTORY:
                createDirectory(journal->j_boolean, id, journal->j_path, true);
                break;
            case MKFILE_PATH:
                createFile(journal->j_path, journal->j_boolean, journal->j_content, true);
                break;
            case MKFILE_SIZE:
                createFile(journal->j_path, journal->j_boolean, journal->j_size, true);
                break;
            case ADDGRP:
                addGroup(id, journal->j_group, true);
                break;
            case DELGRP:
                deleteGroup(id, journal->j_group, true);
                break;
            case ADDUSER:
                addUser(id, journal->j_user, journal->j_content, journal->j_group, true);
                break;
            case DELUSER:
                deleteUser(id, journal->j_user, true);
                break;
            case EDIT_FILE:
                editFile(journal->j_path, journal->j_content, true);
                break;
            default:
                break;
        }

        contador++;
    }

    fclose(myFile);
    return true;
}

bool isSessionActive() {
    if (active_sesion->user != NULL) {
        //cout<<active_sesion->user<<endl;
        return true;
    }
    if (active_sesion->path != NULL) {
        //cout<<active_sesion->path<<endl;
        return true;
    }
    if (active_sesion->namePartition != NULL) {
        //cout<<active_sesion->namePartition<<endl;
        return true;
    }
    if (active_sesion->id != NULL) {
        //cout<<active_sesion->id<<endl;
        return true;
    }
    return false;
}

struct LoginParams {
    char *user;
    char *pass;
    char *id;
};

LoginParams separarParamsLogin(string datos[3]) {
    LoginParams paramsReturn;
    string params[3];
    string retorno[3];
    for (int i = 0; i < 3; ++i) {
        params[i] = datos[i];
        if (params[i][0] == '8' && params[i][1] == '9') {
            retorno[0] = params[i];
//            strcpy(paramsReturn.id, params[i].c_str());
        } else if (params[i][0] == '#') {
            retorno[1] = params[i];
//            string param = params[i].substr(1, params[i].length() - 1);
//            strcpy(paramsReturn.id, param.c_str());
        } else {
            retorno[2] = params[i];
        }
    }
    char aux[1000];
    strcpy(aux,retorno[0].c_str());
    paramsReturn.id =aux;
    char aux1[1000];
    string param = retorno[1].substr(1, retorno[1].length() - 1);
    strcpy(aux1,param.c_str());
    paramsReturn.user =aux1;
    char aux2[1000];
    strcpy(aux2,retorno[2].c_str());
    paramsReturn.pass =aux2;
    return paramsReturn;
}

void loginn(char user[], char pass[], char id[]) {
    active_sesion->user = user;
    active_sesion->id = id;
    string activePathString = getPathDisk(id);
    char activePath[100];
    strcpy(activePath, activePathString.c_str());
    active_sesion->path = activePath;
    string activeNameString = getNamePartition(id);
    char activeName[100];
    strcpy(activeName, activeNameString.c_str());
    active_sesion->namePartition = activeName;
    User *usr = getUser(user, active_sesion->path, active_sesion->namePartition);
    active_sesion->idUser = &usr->id[0];
    active_sesion->idGrp = &usr->group[0];
    cout<<"inicio de sesion correcto"<<endl;
}

#ifndef PROYECTO1_MKFS_H
#define PROYECTO1_MKFS_H

#endif //PROYECTO1_MKFS_H
