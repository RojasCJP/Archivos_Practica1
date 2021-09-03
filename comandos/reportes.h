#include <iostream>
#include <string>
#include "mkfs.h"

using namespace std;

string convertToString(char *a) {
    int i;
    string s = "";
    for (i = 0; i < (sizeof(a) / sizeof(char)); i++) {
        s = s + a[i];
    }
    return s;
}

void reporteDisk(string spath,string id) {
    string path = getPathDisk(id);
    string pathOutput = spath;
    for (int i = 0; i < path.length(); i++) {
        path[i] = tolower(spath[i]);
    }

    string completePath = rpath + path;
    if (!exist_file(completePath)) {
        cout << "disco no encontrado" << endl;
    }
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);

    string particion1;
    string particion2;
    string particion3;
    string particion4;
    int libre1;
    int libre2;
    int libre3;
    int libre4;
    int libre5;
    string espacioLibre1;
    string espacioLibre2;
    string espacioLibre3;
    string espacioLibre4;
    string espacioLibre5;

    Partition particionesOrden[4];
    bool nombreParticion = false;
    for (int i = 0; i < 4; i++) {
        particionesOrden[i] = mbr.partition[i];
    }
    Partition auxiliar;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (particionesOrden[j].start > particionesOrden[j + 1].start) {
                auxiliar = particionesOrden[j];
                particionesOrden[j] = particionesOrden[j + 1];
                particionesOrden[j + 1] = auxiliar;
            }
        }
    }

    particion1 = (particionesOrden[0].start != mbr.size) ? ("|" + convertToString(particionesOrden[0].name) + "\\n" +
                                                            to_string(particionesOrden[0].size / (double) mbr.size))
                                                         : "";
    particion2 = (particionesOrden[1].start != mbr.size) ? ("|" + convertToString(particionesOrden[1].name) + "\\n" +
                                                            to_string(particionesOrden[1].size / (double) mbr.size))
                                                         : "";
    particion3 = (particionesOrden[2].start != mbr.size) ? ("|" + convertToString(particionesOrden[2].name) + "\\n" +
                                                            to_string(particionesOrden[2].size / (double) mbr.size))
                                                         : "";
    particion4 = (particionesOrden[3].start != mbr.size) ? ("|" + convertToString(particionesOrden[3].name) + "\\n" +
                                                            to_string(particionesOrden[3].size / (double) mbr.size))
                                                         : "";

    libre1 = particionesOrden[0].start - sizeof(MBR);
    libre2 = particionesOrden[1].start - (particionesOrden[0].start + particionesOrden[0].size);
    libre3 = particionesOrden[2].start - (particionesOrden[1].start + particionesOrden[1].size);
    libre4 = particionesOrden[3].start - (particionesOrden[2].start + particionesOrden[2].size);
    libre5 = mbr.size - (particionesOrden[3].start + particionesOrden[3].size);

    espacioLibre1 = libre1 != 0 ? ("|libre\\n" + to_string(libre1 / (double) mbr.size)) : "";
    espacioLibre2 = libre2 != 0 ? ("|libre\\n" + to_string(libre2 / (double) mbr.size)) : "";
    espacioLibre3 = libre3 != 0 ? ("|libre\\n" + to_string(libre3 / (double) mbr.size)) : "";
    espacioLibre4 = libre4 != 0 ? ("|libre\\n" + to_string(libre4 / (double) mbr.size)) : "";
    espacioLibre5 = libre5 != 0 ? ("|libre\\n" + to_string(libre5 / (double) mbr.size)) : "";

    string inicioReporte = "digraph g {\n        rankdir = LR;\n    subgraph cluster0\n    {\n        ";
    string discoReporte = "Array [ shape = record, label = \"{";
    string medioReporte = "MBR" + espacioLibre1 + particion1 + espacioLibre2 + particion2 + espacioLibre3 + particion3 +
                          espacioLibre4 + particion4 + espacioLibre5;
    string finalReporte = "}\"] ;\n    }\n}";

    string reporteCompleto = inicioReporte + discoReporte + medioReporte + finalReporte;
    //todo tengo que hacer que corra el graphviz para que genere el archivo
    cout << reporteCompleto << endl;
}

void reporteMBR(string spath,string id) {
    string path = getPathDisk(id);
    string pathOutput = spath;
    for (int i = 0; i < path.length(); i++) {
        path[i] = tolower(spath[i]);
    }

    string completePath = rpath + path;
    if (!exist_file(completePath)) {
        cout << "disco no encontrado" << endl;
    }
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);

    string size = to_string(mbr.size);
    string creationDate = asctime(gmtime(&mbr.creationDate));
    string diskSignature = to_string(mbr.diskSignature);
    char fit = mbr.fit;

    string inicioReporte= "digraph test {\n"
                          "    graph [ratio=fill];\n"
                          "    node [label=\"\\N\", fontsize=15, shape=plaintext];\n"
                          "    graph [bb=\"0,0,352,154\"];\n"
                          "    arset [label=<\n"
                          "        <TABLE ALIGN=\"LEFT\">";
    string medioReporte = "<TR>\n"
                          "                <TD>Nombre</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>Valor</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "            <TR>\n"
                          "                <TD>Tamano MBR</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+size+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "            <TR>\n"
                          "                <TD>Fecha creacion MBR</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+creationDate+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "            <TR>\n"
                          "                <TD>Disk Signature MBR</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+diskSignature+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "            <TR>\n"
                          "                <TD>Fit Disk</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+fit+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>";
    string finalReporte = "        </TABLE>\n"
                          "    >, ];\n"
                          "}";

    for (int i = 0; i < 4; ++i) {
        char status = mbr.partition[i].status;
        char type = mbr.partition[i].type;
        char fitp = mbr.partition[i].fit;
        int sizep = mbr.partition[i].size;
        string name = mbr.partition[i].name;
        if (mbr.partition[i].status != '0') {
            medioReporte+="<TR>\n"
                          "                <TD>Part "+to_string(i)+" Status</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>1</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "            <TR>\n"
                          "                <TD>Part "+to_string(i)+" Type</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+type+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "            <TR>\n"
                          "                <TD>Part "+to_string(i)+" Fit</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+fitp+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "             <TR>\n"
                          "                <TD>Part "+to_string(i)+" Size</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+ to_string(sizep)+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>\n"
                          "             <TR>\n"
                          "                <TD>Part "+to_string(i)+" Name</TD>\n"
                          "                <TD>\n"
                          "                    <TABLE BORDER=\"0\">\n"
                          "                        <TR><TD>"+name+"</TD></TR>\n"
                          "                    </TABLE>\n"
                          "                </TD>\n"
                          "            </TR>";
        }
    }
    string reporteCompleto = inicioReporte+medioReporte+finalReporte;
    cout<<reporteCompleto<<endl;
}

bool reportBitmap(int rep, char path[], char name[],char path_rep[]){
    string completePath = rpath+path;
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    SuperBlock *sb = readSuperBlock(sc,name);
    if(sb == NULL){
        return false;
    }
    FILE * myFile;
    myFile = fopen (sc,"rb+");
    if (myFile==NULL)
    {
        cout<<"Error al abrir el disco\n";
        return false;
    }

    FILE * fileReport;
    fileReport = fopen (path_rep,"w+");
    if (fileReport==NULL)
    {
        cout<<"Error al crear archivo de reporte\n";
        return false;
    }
    fseek(fileReport, 0, SEEK_SET);
    if(rep == 0){
        //reporte de inodos
        char caracter;
        int contador = 0;
        fseek(myFile, sb->bmInodeStart, SEEK_SET);
        while(contador<sb->inodesCount){
            fread(&caracter, sizeof(char), 1, myFile);
            fwrite(&caracter, sizeof(char), 1, fileReport);
            if((contador+1)%20==0 && contador!=0){
                fwrite("\n", sizeof(char), 1, fileReport);
            }
            contador++;
        }
    }else{
        //reporte de bloques
        char caracter;
        int contador = 0;
        fseek(myFile, sb->bmBlockStart, SEEK_SET);
        while(contador<sb->blocksCount){
            fread(&caracter, sizeof(char), 1, myFile);
            fwrite(&caracter, sizeof(char), 1, fileReport);
            if((contador+1)%20==0 && contador!=0){
                fwrite("\n", sizeof(char), 1, fileReport);
            }
            contador++;
        }
    }
    fclose (fileReport);
    fclose (myFile);
    delete sb;
    return true;
}

bool reportSuperBlock(char path[], char name[],char path_rep[]){
    string completePath = rpath+path;
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    SuperBlock *sb = readSuperBlock(sc,name);
    string reporteBody = "digraph test {\n"
                         "    graph [ratio=fill];\n"
                         "    node [label=\"\\N\", fontsize=15, shape=plaintext];\n"
                         "    graph [bb=\"0,0,352,154\"];\n"
                         "    arset [label=<\n"
                         "        <TABLE ALIGN=\"LEFT\">\n"
                         "            <TR>\n"
                         "                <TD>Nombre</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>Valor</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Inodes Count</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+to_string(sb->inodesCount)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Blocks Count</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->blocksCount)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "           <TR>\n"
                         "                <TD>Free Inodes Count</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->freeInodesCount)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Free Blocks Count</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->freeBlocksCount)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Mtime</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+asctime(gmtime(&sb->mtime))+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Umtime</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+asctime(gmtime(&sb->umtime))+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Magic</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+to_string(sb->magic)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Inode Size</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->inodeSize)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Block Size</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->blockSize)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>First Inode</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->firstInode)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Frist Block</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->firstBlock)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>BM Inode Start</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->bmInodeStart)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>BM Block Start</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->bmBlockStart)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Inode Start</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->inodeStart)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Block Start</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>"+ to_string(sb->blockStart)+"</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "        </TABLE>\n"
                         "    >, ];\n"
                         "}";
                         cout<<reporteBody<<endl;
                         return true;
}


//todo hacer el reporte con los ebr y mostrarlos en el reporte de disco