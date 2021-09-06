
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

SuperBlock* readSuperBlock(char path[], char name[]){
    MBR *disco = openMBR(path);
    if(disco==NULL){
        std::cout<<"Error al leer el disco\n";
        return NULL;
    }
    int init;
    bool res = getStartPartition(disco,name,&init);
    if(res!=true){
        return NULL;
    }
    delete disco;
    FILE *myFile = fopen(path,"rb+");
    if(myFile==NULL){
        cout<<"Error al abrir el disco \n";
        return NULL;
    }
    SuperBlock *sb = (SuperBlock*)malloc(sizeof(SuperBlock));

    fseek(myFile, init, SEEK_SET);
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

BLOCK_POINTER* readBlockPointer(char path[], int init){
    FILE *myFile = fopen(path,"rb+");
    if(myFile==NULL){
        cout<<"Error al abrir el disco \n";
        return NULL;
    }
    BLOCK_POINTER *bd = (BLOCK_POINTER*)malloc(sizeof(BLOCK_POINTER));

    fseek(myFile, init, SEEK_SET);
    fread(bd, sizeof(BLOCK_POINTER), 1, myFile);
    fclose(myFile);
    return bd;
}

BLOCK_POINTER* getNewBlockPointer(){
    BLOCK_POINTER *bl = (BLOCK_POINTER*)malloc(sizeof(BLOCK_POINTER));
    int j;
    for(j = 0;j<16;j++){
        bl->pointers[j]=-1;
    }
    return bl;
}

void writeBlockPointer(BLOCK_POINTER *newPoints,char path[],int init){
    FILE * myFile;
    myFile = fopen (path,"rb+");
    if (myFile==NULL)
    {
        cout<<"Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(newPoints, sizeof(BLOCK_POINTER), 1, myFile);
    //cerrando stream
    fclose (myFile);
}

int createPointersInd(int idPointBlock,int *idBloqueActual,SuperBlock *sb,char path[]){
    if(idPointBlock-11 == 1) {
        BLOCK_POINTER *newPoints = getNewBlockPointer();
        int idBloque = sb->firstBlock;
        writeBlockPointer(newPoints,path,getInitBlock(sb,sb->firstBlock));
        sb->firstBlock = getBitmapIndex(sb->bmBlockStart,sb->blocksCount,path);
        sb->freeBlocksCount--;
        *idBloqueActual = idBloque;
        return idBloque;
    }else{
        int res = createPointersInd(idPointBlock-1,idBloqueActual,sb,path);
        BLOCK_POINTER *pointers = getNewBlockPointer();
        pointers->pointers[0] = res;
        res = sb->firstBlock;
        writeBlockPointer(pointers,path,getInitBlock(sb,sb->firstBlock));
        sb->firstBlock = getBitmapIndex(sb->bmBlockStart,sb->blocksCount,path);
        sb->freeBlocksCount--;
        return res;
    }
    return -1;
}

int getFreeIndexFromBlockPointer(int nivel,INODO *inodo,int indexBloqueActual,char path[],SuperBlock *sb,int *indexFree){
    if(inodo->block[nivel]!=-1){
        if(nivel == 12){
            BLOCK_POINTER *pointers = readBlockPointer(path,getInitBlock(sb,indexBloqueActual));
            if(pointers == NULL){
                return 0;
            }
            int indexInBlockP;
            for(indexInBlockP = 0;indexInBlockP<15;indexInBlockP++){
                if(pointers->pointers[indexInBlockP] == -1){
                    *indexFree = indexInBlockP;
                    return indexBloqueActual;
                }
            }
        }else{
            BLOCK_POINTER *pointers = readBlockPointer(path,getInitBlock(sb,indexBloqueActual));
            if(pointers == NULL){
                return 0;
            }
            int indexInBlockP;
            for(indexInBlockP = 0;indexInBlockP<15;indexInBlockP++){
                if(pointers->pointers[indexInBlockP] != -1){
                    int r = getFreeIndexFromBlockPointer(nivel-1, inodo, pointers->pointers[indexInBlockP], path, sb, indexFree);
                    if(r!=-1)return r;
                }else{
                    int bloquePadre = indexBloqueActual;
                    int bloque = createPointersInd(nivel-1,&indexBloqueActual,sb,path);
                    if(bloque== -1) return 0;
                    pointers->pointers[indexInBlockP] = bloque;
                    writeBlockPointer(pointers,path,getInitBlock(sb,bloquePadre));
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

BLOCK_FILE* getNewBlockFile(){
    BLOCK_FILE *bl = (BLOCK_FILE*)malloc(sizeof(BLOCK_FILE));
    clearArray(bl->content,64);;
    return bl;
}

void writeBlockFile(BLOCK_FILE *sb,char path[],int init){
    FILE * myFile;
    myFile = fopen (path,"rb+");
    if (myFile==NULL)
    {
        cout<<"Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(sb, sizeof(BLOCK_FILE), 1, myFile);
    //cerrando stream
    fclose (myFile);
}

void saveBlockFile(BLOCK_FILE *block,SuperBlock *sb,char path[]){
    writeBlockFile(block,path,getInitBlock(sb,sb->firstBlock));
    sb->firstBlock = getBitmapIndex(sb->blockStart,sb->blocksCount,path);
    sb->freeBlocksCount--;
}

bool addFileBlockPointers(INODO *inodo,int *indexofInodo,BLOCK_FILE *block,SuperBlock *sb,char path[]){
    if(inodo->block[*indexofInodo]!=-1){
        int free= 0;
        int res =getFreeIndexFromBlockPointer(*indexofInodo,inodo,inodo->block[*indexofInodo],path,sb,&free);
        if(res==-1){
            return false;
        }
        BLOCK_POINTER *point = readBlockPointer(path,getInitBlock(sb,res));
        point->pointers[free] = sb->firstBlock;
        writeBlockPointer(point,path,getInitBlock(sb,res));
        saveBlockFile(block,sb,path);
    }else{
        //CREAR NUEVO BLOQUE
        int nuevoIndex;
        int bloque = createPointersInd(*indexofInodo,&nuevoIndex,sb,path);
        if(bloque== -1) return false;
        inodo->block[*indexofInodo] = bloque;
        BLOCK_POINTER *point = readBlockPointer(path,getInitBlock(sb,nuevoIndex));
        point->pointers[0] = sb->firstBlock;
        writeBlockPointer(point,path,getInitBlock(sb,nuevoIndex));
        //CREAR BLOQUE CON CONTENIDO
        saveBlockFile(block,sb,path);
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

int findDirectoryInPointers(int level,int indexBlock,int *indexInodoActual,char namedir[],SuperBlock *sb,char path[]){

    if(level == 1){
        BLOCK_POINTER *pointers = readBlockPointer(path,getInitBlock(sb,indexBlock));
        int indexofPointer;
        for(indexofPointer = 0;indexofPointer<14;indexofPointer++){
            if(pointers->pointers[indexofPointer]!=-1){
                //BUSCANDO EN INODO
                INODO *inodo = readInodo(path,getInitInode(sb,pointers->pointers[indexofPointer]));
                if(inodo==NULL)return -1;
                int indexBlock;
                BLOCK_DIR *block;
                for(indexBlock = 0;indexBlock<12;indexBlock++){
                    if(inodo->block[indexBlock]!=-1){
                        if(inodo->type == 1){
                            block = readBlockDirectory(path,getInitBlock(sb,inodo->block[indexBlock]));
                            if(block == NULL)return -1;
                            int i=0;
                            if(block->content[i].inodo!=-1){
                                if(strcmp(block->content[i].name,namedir)==0){
                                    *indexInodoActual = block->content[0].inodo;
                                    return inodo->block[indexBlock];
                                }
                            }
                        }
                    }
                }
            }
        }

    }else{
        BLOCK_POINTER *pointers = readBlockPointer(path,getInitBlock(sb,indexBlock));
        int indexofPointer;
        for(indexofPointer = 0;indexofPointer<14;indexofPointer++){
            if(pointers->pointers[indexofPointer]!=-1){
                int i = findDirectoryInPointers(level-1,pointers->pointers[indexofPointer],indexInodoActual,namedir,sb,path);
                if(i!=-1){
                    return i;
                }
            }
        }

    }

    return -1;
}

int findDirectory(char namedir[],char path[],int *indexInodoActual,SuperBlock *sb){
    INODO *inodo = readInodo(path,getInitInode(sb,*indexInodoActual));
    if(inodo==NULL){
        return -1;
    }
    int indexBlock;
    BLOCK_DIR *block;
    for(indexBlock = 0;indexBlock<12;indexBlock++){
        if(inodo->block[indexBlock]!=-1){
            if(inodo->type == 1){
                block = readBlockDirectory(path,getInitBlock(sb,inodo->block[indexBlock]));
                if(block == NULL){
                    return -1;
                }
                int i;
                for(i=2;i<4;i++){
                    if(block->content[i].inodo!=-1){
                        if(strcmp(block->content[i].name,namedir)==0){
                            *indexInodoActual = block->content[i].inodo;
                            inodo = readInodo(path,getInitInode(sb,*indexInodoActual));
                            if(inodo==NULL){
                                return -1;
                            }
                            return inodo->block[0];
                        }
                    }
                }
            }
        }
    }
    for(indexBlock = 12;indexBlock<14;indexBlock++){
        int index = findDirectoryInPointers(indexBlock-11,inodo->block[indexBlock],indexInodoActual,namedir,sb,path);
        if(index!=-1)return index;
    }
    return -1;
}
//type
//0=file 1=dir 2=pointer
bool createChildDirectory(char dirPad[],char dirName[],char path[],SuperBlock *sb,int *indexInodoPadre,int *indexBloqueActual){
    int indexFree = -1;
    int type = 1;
    bool res = getFreeIndexDirectory(dirPad,path,sb,indexBloqueActual,indexInodoPadre,&indexFree,&type);
    if(res!=true){
        return res;
    }
    if(indexFree == -1){
        return false;
    }
    int indexnew = writeDirectory(sb,path,dirName,dirPad,*indexInodoPadre);
    if(type == 1){
        BLOCK_DIR *block = readBlockDirectory(path,getInitBlock(sb,*indexBloqueActual));
        block->content[indexFree].inodo = indexnew;
        strcpy(block->content[indexFree].name,dirName);
        writeBlockDirectory(block,path,getInitBlock(sb,*indexBloqueActual));
        delete block;
    }else if(type == 2){
        BLOCK_POINTER *block = readBlockPointer(path,getInitBlock(sb,*indexBloqueActual));
        block->pointers[indexFree] = indexnew;
        writeBlockPointer(block,path,getInitBlock(sb,*indexBloqueActual));
        // delete block;
    }
    *indexInodoPadre = indexnew;

    return true;
}


void writeSuperBlock(SuperBlock *sb,char path[],int init){
    FILE * myFile;
    myFile = fopen (path,"rb+");
    if (myFile==NULL)
    {
        cout<<"Error al abrir el disco\n";
        return;
    }
    fseek(myFile, init, SEEK_SET);
    fwrite(sb, sizeof(SuperBlock), 1, myFile);
    //cerrando stream
    fclose (myFile);
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
    char namecrt[name.size()+1];
    strcpy(namecrt, name.c_str());
    createFileWithText("/users.txt", true, users, 28, pathdir, namecrt);
    createFileWithText("/root/prueba.txt",true,"buenas tardes\n",14,pathdir,namecrt);
    cout << "se creo la carpeta raiz" << endl;
    SBnuevo.freeBlocksCount = SBnuevo.freeBlocksCount - 1;
    SBnuevo.freeInodesCount = SBnuevo.freeInodesCount - 1;
    SBnuevo.firstInode = 1;
    SBnuevo.firstBlock = 1;
    SBnuevo.inodesCount = 1;
    SBnuevo.blocksCount = 1;
}

void createUsers() {
    BLOCK_DIR newDir;
    INODO newInode;
    newDir.content[0].inodo = 0;
    newDir.content[1].inodo = 0;
    strcpy(newDir.content[0].name, "..");
    strcpy(newDir.content[1].name, ".");
    newDir.content[2];
    newDir.content[3];

    newInode.type = '0';
    newInode.uid = 0;
    newInode.gid = 0;
    newInode.size = 0;
    newInode.block[0] = 0;
    newInode.block[1];
    newInode.block[2];
    newInode.block[3];
    newInode.block[4];
    //todo creo que tengo que escribir estos en el archivo, eso ya esta creo
}

//todo tengo que jalar lo de la carpeta root
//todo tengo que probar a ver si funciona dios ayudame



#ifndef PROYECTO1_MKFS_H
#define PROYECTO1_MKFS_H

#endif //PROYECTO1_MKFS_H
