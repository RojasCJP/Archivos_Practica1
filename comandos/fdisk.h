#include <iostream>
#include "mkdisk.h"

using namespace std;

int getMinor(MBR mbr) {
    int indexMinor = 0;
    for (int i = 0; i < 4; i++) {
        if (mbr.partition[i].start < mbr.partition[indexMinor].start) {
            indexMinor = i;
        }
    }
    return indexMinor;
}

ParamsFDisk separarParams(string params[8]) {
    ParamsFDisk paramsFinal;
    string datos[8];
    for (int i = 0; i < 8; i++) {
        datos[i] = params[i];
        for (int j = 0; j < params[i].length(); j++) {
            datos[i][j] = tolower(params[i][j]);
        }

        if (datos[i][0] == '0' || datos[i][0] == '1' || datos[i][0] == '2' || datos[i][0] == '3' ||
            datos[i][0] == '4' || datos[i][0] == '5' || datos[i][0] == '6' || datos[i][0] == '7' ||
            datos[i][0] == '8' || datos[i][0] == '9') {
            paramsFinal.size = stoi(datos[i]);
        } else if ((datos[i][0] == 'b' || datos[i][0] == 'k' || datos[i][0] == 'm') && datos[i].length() == 1) {
            paramsFinal.units = datos[i][0];
        } else if (datos[i][0] == '"') {
            if (datos[i][1] == '/') {
                paramsFinal.path = datos[i];
            } else {
                paramsFinal.name = datos[i];
            }
        } else if (datos[i][0] == '/') {
            paramsFinal.path = datos[i];
        } else if ((datos[i][0] == 'p' || datos[i][0] == 'e' || datos[i][0] == 'l') && datos[i].length() == 1) {
            paramsFinal.type = datos[i][0];
        } else if (datos[i] == "bf" || datos[i] == "ff" || datos[i] == "wf") {
            paramsFinal.fit = datos[i];
        } else if (datos[i][0] == '%') {
            paramsFinal.add = stoi(datos[i].substr(1, datos[i].length() - 1));
        } else if (datos[i] == "fast" || datos[i] == "full") {
            paramsFinal.del = datos[i];
        } else if (datos[i][0] == '#') {
            paramsFinal.name = datos[i].substr(1, datos[i].length() - 1);
        }
    }
    return paramsFinal;
}

int getIndexFollow(MBR mbr, int index) {
    int indexReturn = index;
    int sizeBetween = INT32_MAX;
    int finalIndex = mbr.partition[index].start + mbr.partition[index].size;
    for (int i = 0; i < 4; i++) {
        if (finalIndex < mbr.partition[i].start && (mbr.partition[i].start - finalIndex) < sizeBetween) {
            sizeBetween = mbr.partition[i].start - finalIndex;
            indexReturn = i;
        }
    }
    return indexReturn;
}

void makePartition(ParamsFDisk params) {
    int size = params.size;
    char units = params.units;
    string path = params.path;
    char type = params.type;
    string fit = params.fit;
    string del = params.del;
    string name = params.name;
    int add = params.add;
    if (add < -10000 || add > 10000) {
        add = 0;
    }
    if (units == 'b') {
        size = params.size;
    } else if (units == 'k') {
        size = params.size * 1024;
    } else if (units == 'm') {
        size = params.size * 1024 * 1024;
    } else {
        size = params.size * 1024;
    }

    string completePath = rpath + path;
    if (!exist_file(completePath)) {
        cout << "disco no encontrado" << endl;
        cout << "revisa la ruta: " << completePath << endl;
    }
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);
    cout
            << "------------------------------------------------disco encontrado------------------------------------------------"
            << endl;
    cout << "Fecha creacion: " << asctime(gmtime(&mbr.creationDate)) << endl;
    cout << "Signature: " << mbr.diskSignature << endl;
    cout << "Tamano: " << mbr.size << endl;
    cout << "Fit: " << mbr.fit << endl;

    int index = 0;
    bool existente = false;
    for (int i = 0; i < 4; i++) {
        if (mbr.partition[i].name == name) {
            existente = true;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (add == 0 && del == "") {
            if (mbr.partition[i].status == '0' && !existente) {
                index = i;
                break;
            }
        } else {
            if (mbr.partition[i].name == name) {
                index = i;
                break;
            }
        }
    }
    if (add == 0 && del == "") {
        if (mbr.partition[index].status == '0') {

            if (type == 'p' || type == 'e') {
                mbr.partition[index].status = '1';
                mbr.partition[index].size = size;
            }
            if (fit == "bf") {
                mbr.partition[index].fit = 'b';
            } else if (fit == "wf") {
                mbr.partition[index].fit = 'w';
            } else {
                mbr.partition[index].fit = 'f';
            }
            if (type == 'p' || type == 'e') {
                if (type == 'p') {
                    mbr.partition[index].type = 'p';
                } else {
                    bool existExtended = false;
                    for (int v = 0; v < 4; ++v) {
                        if (mbr.partition[v].type == 'e') {
                            existExtended = true;
                        }
                    }
                    if (existExtended) {
                        cout << "ya existe una particion extendida no se puede crear otra" << endl;
                        return;
                    }
                    mbr.partition[index].type = 'e';
                }

                if (mbr.partition[index].fit == 'f') {
                    bool trigger = false;
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            if (!trigger) {
                                int posibleInicial = mbr.partition[getMinor(mbr)].start - sizeof(MBR);
                                int posible = mbr.partition[getIndexFollow(mbr, i)].start -
                                              (mbr.partition[i].start + mbr.partition[i].size);
                                if (posibleInicial > mbr.partition[index].size) {
                                    mbr.partition[index].start = sizeof(MBR) + 2;
                                    trigger = true;
                                    break;
                                } else if (posible > mbr.partition[index].size) {
                                    mbr.partition[index].start = mbr.partition[i].start + mbr.partition[i].size + 1;
                                    trigger = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (!trigger) {
                        cout << "no hay espacio para crear la particion" << endl;
                        mbr.partition[index].status = '0';
                        mbr.partition[index].size = 0;
                        mbr.partition[index].fit = 'f';
                        mbr.partition[index].start = mbr.size;
                        strcpy(mbr.partition[index].name, "");
                        return;
                    }
                } else if (mbr.partition[index].fit == 'b') {
                    int indexMejor = 12;
                    int inicial = mbr.partition[getMinor(mbr)].start - sizeof(MBR);
                    int definitivo;
                    if (inicial > mbr.partition[index].size) {
                        definitivo = inicial;
                    } else {
                        definitivo = INT32_MAX;
                    }
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            int posible = mbr.partition[j].start - (mbr.partition[i].start + mbr.partition[i].size);
                            if (posible < definitivo && posible > mbr.partition[index].size) {
                                indexMejor = i;
                                definitivo = posible;
                            }
                        }
                    }
                    if (definitivo != INT32_MAX) {
                        if (indexMejor == 12) {
                            mbr.partition[index].start = sizeof(MBR) + 2;
                        } else {
                            mbr.partition[index].start =
                                    mbr.partition[indexMejor].start + mbr.partition[indexMejor].size + 1;
                        }
                    } else {
                        cout << "no hay espacio para crear la particion" << endl;
                        mbr.partition[index].status = '0';
                        mbr.partition[index].size = 0;
                        mbr.partition[index].fit = 'f';
                        mbr.partition[index].start = mbr.size;
                        strcpy(mbr.partition[index].name, "");
                        return;
                    }
                } else {
                    int indexMejor = 12;
                    int inicial = mbr.partition[getMinor(mbr)].start - sizeof(MBR);
                    int definitivo;
                    if (inicial > mbr.partition[index].size) {
                        definitivo = inicial;
                    } else {
                        definitivo = INT32_MIN;
                    }
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            int posible = mbr.partition[getIndexFollow(mbr, i)].start -
                                          (mbr.partition[i].start + mbr.partition[i].size);
                            if (posible > definitivo && posible > mbr.partition[index].size) {
                                indexMejor = i;
                                definitivo = posible;
                            }
                        }
                    }
                    if (definitivo != INT32_MIN) {
                        if (indexMejor == 12) {
                            mbr.partition[index].start = sizeof(MBR) + 2;
                        } else {
                            mbr.partition[index].start =
                                    mbr.partition[indexMejor].start + mbr.partition[indexMejor].size + 1;
                        }
                    } else {
                        cout << "no hay espacio para crear la particion" << endl;
                        mbr.partition[index].status = '0';
                        mbr.partition[index].size = 0;
                        mbr.partition[index].fit = 'f';
                        mbr.partition[index].start = mbr.size;
                        strcpy(mbr.partition[index].name, "");
                        return;
                    }
                }
            } else {
                file = fopen(sc, "rb+");
                bool existExtended = false;
                int indexExtended = 0;
                for (int v = 0; v < 4; ++v) {
                    if (mbr.partition[v].type == 'e') {
                        existExtended = true;
                        indexExtended = v;
                    }
                }
                if (!existExtended) {
                    cout << "no se puede crear una particion logica porque no existe ninguna particion extendida"
                         << endl;
                    return;
                }
                EBR logicPartition;
                logicPartition.size = size;
                logicPartition.name = name;
                fseek(file, mbr.partition[indexExtended].start + 1, SEEK_SET);
                EBR auxiliar;
                fread(&auxiliar, sizeof(EBR), 1, file);
                int startExistente = 0;
                while (auxiliar.siguiente == (auxiliar.start + auxiliar.size + 1)) {
                    startExistente = auxiliar.start;
                    fseek(file, auxiliar.siguiente, SEEK_SET);
                    fread(&auxiliar, sizeof(auxiliar), 1, file);
                }
                if (startExistente != 0) {
                    fseek(file, startExistente, SEEK_SET);
                    fread(&auxiliar, sizeof(auxiliar), 1, file);
                    logicPartition.start = auxiliar.siguiente;
                    logicPartition.siguiente = logicPartition.start + logicPartition.size + 1;
                } else {
                    logicPartition.start = mbr.partition[indexExtended].start + 1;
                    logicPartition.siguiente = logicPartition.start + logicPartition.size + 1;
                }
                fseek(file, logicPartition.start, SEEK_SET);
                fwrite(&logicPartition, sizeof(EBR), 1, file);
                cout<<"particion logica creada con exito"<<endl;
                auxiliar = logicPartition;
            }
            if (type == 'p' || type == 'e') {
                strcpy(mbr.partition[index].name, name.c_str());
                cout <<"la particion "<<mbr.partition[index].name <<" comienza en "<<mbr.partition[index].start<<endl;
                cout << "particion creada con exito " << mbr.partition[index].name << endl;
            }
        } else {
            cout << "no se puede crear una particion" << endl;
            cout << mbr.partition[index].status << endl;
            cout << mbr.partition[index].start << endl;
            cout << mbr.partition[index].size << endl;
            cout << mbr.partition[index].fit << endl;
        }
    } else if (del != "") {
        if (del == "full") {
            file = fopen(sc, "rb+");
            fseek(file, mbr.partition[index].start, SEEK_SET);
            for (int i = 0; i < mbr.partition[index].size; ++i) {
                fwrite("\0", 1, 1, file);
            }
            cout << mbr.partition[index].size << "tamano de la particion que voy a eliminar" << endl;
            mbr.partition[index].status = '0';
            mbr.partition[index].size = 0;
            mbr.partition[index].fit = 'f';
            mbr.partition[index].start = mbr.size;
            strcpy(mbr.partition[index].name, "");
        } else if (del == "fast") {
            mbr.partition[index].status = '0';
            mbr.partition[index].size = 0;
            mbr.partition[index].fit = 'f';
            mbr.partition[index].start = mbr.size;
            strcpy(mbr.partition[index].name, "");
        }
    } else if (add != 0) {
        bool posible = false;
        int siguientePart = getIndexFollow(mbr, index);
        int finalPart = mbr.partition[index].start + mbr.partition[index].size;
        for (int i = 0; i < 4; i++) {
            if (finalPart < mbr.partition[i].start && mbr.partition[i].start < mbr.partition[siguientePart].start) {
                siguientePart = i;
            }
        }
        int cantidadAdd = add;
        if (units == 'b') {
            cantidadAdd = add;
        } else if (units == 'k') {
            cantidadAdd = add * 1024;
        } else if (units == 'm') {
            cantidadAdd = add * 1024 * 1024;
        } else {
            cantidadAdd = add * 1024;
        }
        if ((mbr.partition[siguientePart].start - finalPart) > add) {
            mbr.partition[index].size = mbr.partition[index].size + cantidadAdd;
        }
    }
    file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, file);
    fclose(file);
}
