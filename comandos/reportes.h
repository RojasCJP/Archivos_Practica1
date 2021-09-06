#include <iostream>
#include <string>
#include "mkfs.h"

using namespace std;


void graphBlockPointer(int level, int indexPadre, int indexBlock, FILE *fileReport, char path[], SuperBlock *sb,
                       bool graphConnection, int type);

void graphConnectionInodoBloque(int indexnodo, int indexBloque, int indexPuerto, FILE *myFile);

void graphBlockFile(BLOCK_FILE *block, int initBlock, FILE *myFile, int indexInodo);

void graphBlockDirectory(BLOCK_DIR *block, int initBlock, FILE *myFile, int indexInodo, SuperBlock *sb, char path[],
                         bool graphConnection);

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

void graphInodo(INODO *inodo, int indexInodo, FILE *myFile, char path[], SuperBlock *sb) {
    if (inodo == NULL) return;
    fputs("i_", myFile);
    fputs(&to_string(indexInodo)[0], myFile);
    fputs("[ shape=plaintext label=< \n", myFile);
    fputs("<table border='0' cellborder='1' cellspacing='0'>\n", myFile);
    fputs("<tr><td ", myFile);
    fputs("port=\"ib", myFile);
    fputs(&to_string(indexInodo * sizeof(INODO))[0], myFile);
    fputs("\"", myFile);
    fputs(" colspan=\"3\">Inodo ", myFile);
    fputs(&to_string(indexInodo)[0], myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_uid</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(&to_string(inodo->uid)[0], myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_gid</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(&to_string(inodo->gid)[0], myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_size</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(&to_string(inodo->size)[0], myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_atime</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(asctime(gmtime(&inodo->atime)), myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_ctime</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(asctime(gmtime(&inodo->ctime)), myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_mtime</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(asctime(gmtime(&inodo->mtime)), myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_type</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(&to_string(inodo->type)[0], myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td bgcolor=\"#fbffa8\">i_perm</td><td bgcolor=\"#fbffa8\">", myFile);
    fputs(&to_string(inodo->permisos)[0], myFile);
    fputs("</td></tr>\n", myFile);
    int i = 0;
    //APUNTADORES DIRECTOS
    while (i < 12) {
        fputs("<tr><td bgcolor=\"#ffd1a8\">AD", myFile);
        fputs(&to_string(i + 1)[0], myFile);
        fputs("</td><td ", myFile);
        if (inodo->block[i] != -1) {
            fputs("port=\"", myFile);
            fputs(&to_string((indexInodo + inodo->block[i]) * sizeof(INODO))[0], myFile);
            fputs("\"", myFile);
        }
        fputs(" bgcolor=\"#ffd1a8\">", myFile);
        fputs(&to_string(inodo->block[i])[0], myFile);
        fputs("</td></tr>\n", myFile);
        i++;
    }

    while (i < 15) {
        fputs("<tr><td bgcolor=\"#a8ffdf\">AI", myFile);
        fputs(&to_string(i - 11)[0], myFile);
        fputs("</td><td ", myFile);
        if (inodo->block[i] != -1) {
            fputs("port=\"", myFile);
            fputs(&to_string((indexInodo + inodo->block[i]) * sizeof(INODO))[0], myFile);
            fputs("\"", myFile);
        }
        fputs(" bgcolor=\"#a8ffdf\">", myFile);
        fputs(&to_string(inodo->block[i])[0], myFile);
        fputs("</td></tr>\n", myFile);
        i++;
    }

    fputs("</table>\n", myFile);
    fputs(">];\n", myFile);

    //GRAFICAR BLOQUES
    i = 0;
    while (i < 12) {
        if (inodo->block[i] != -1) {
            int pos = getInitBlock(sb, inodo->block[i]);
            if (inodo->type == 1) {
                BLOCK_DIR *dir = readBlockDirectory(path, pos);
                if (dir != NULL) {
                    graphBlockDirectory(dir, inodo->block[i], myFile, indexInodo, sb, path, true);
                }
            } else if (inodo->type == 0) {
                BLOCK_FILE *dir = readBlockFile(path, pos);
                if (dir != NULL) {
                    graphBlockFile(dir, inodo->block[i], myFile, indexInodo);
                }
            }
            graphConnectionInodoBloque(indexInodo, inodo->block[i], (indexInodo + inodo->block[i]) * sizeof(INODO),
                                       myFile);
        }
        i++;
    }
    while (i < 15) {
        if (inodo->block[i] != -1) {
            graphBlockPointer(i - 11, indexInodo, inodo->block[i], myFile, path, sb, true, inodo->type);
            graphConnectionInodoBloque(indexInodo, inodo->block[i], (indexInodo + inodo->block[i]) * sizeof(INODO),
                                       myFile);
        }
        i++;
    }
}

void graphBlockFile(BLOCK_FILE *block, int initBlock, FILE *myFile, int indexInodo) {
    fputs("b_", myFile);
    fputs(&to_string(initBlock)[0], myFile);
    fputs("[ shape=plaintext label=< \n", myFile);
    fputs("<table border='0' cellborder='1' cellspacing='0'>\n", myFile);
    fputs("<tr><td ", myFile);
    fputs("port=\"", myFile);
    fputs(&to_string((indexInodo + initBlock) * sizeof(INODO))[0], myFile);
    fputs("\">Bloque ", myFile);
    fputs(&to_string(initBlock)[0], myFile);
    fputs("</td></tr>\n", myFile);
    fputs("<tr><td>", myFile);
    int i;
    for (i = 0; i < 64; i++) {
        if (block->content[i] != '\0' && block->content[i] > 0) {
            fprintf(myFile, "%c", block->content[i]);
            if (i != 0) {
                if (i % 31 == 0) {
                    fputs("<br/>", myFile);
                }
            }
        }
    }
    fputs("</td></tr>\n", myFile);
    fputs("</table>\n", myFile);
    fputs(">];\n", myFile);
}

void graphConnectionBloqueInodo(int indexnodo, int indexBloque, int indexPuerto, FILE *myFile) {
    fputs("b_", myFile);
    fputs(&to_string(indexBloque)[0], myFile);
    fputs(":", myFile);
    fputs("ib", myFile);
    fputs(&to_string(indexPuerto)[0], myFile);
    fputs("->", myFile);
    fputs("i_", myFile);
    fputs(&to_string(indexnodo)[0], myFile);
    fputs(":", myFile);
    fputs("ib", myFile);
    fputs(&to_string(indexPuerto)[0], myFile);
    fputs(";\n", myFile);
}

void graphBlockDirectory(BLOCK_DIR *block, int initBlock, FILE *myFile, int indexInodo, SuperBlock *sb, char path[],
                         bool graphConnection) {
    fputs("b_", myFile);
    fputs(&to_string(initBlock)[0], myFile);
    fputs("[ shape=plaintext label=< \n", myFile);
    fputs("<table border='0' cellborder='1' cellspacing='0'>\n", myFile);
    fputs("<tr><td ", myFile);
    fputs("port=\"", myFile);
    fputs(&to_string((indexInodo + initBlock) * sizeof(INODO))[0], myFile);
    fputs("\"", myFile);
    fputs(" colspan=\"3\">Bloque ", myFile);
    fputs(&to_string(initBlock)[0], myFile);
    fputs("</td></tr>\n", myFile);

    char colors[4][10];
    clearArray(colors[0], 10);
    strcat(colors[0], "#e5b7ff");
    clearArray(colors[1], 10);
    strcat(colors[1], "#c0ffb7");
    clearArray(colors[2], 10);
    strcat(colors[2], "#ffb7c0");
    clearArray(colors[3], 10);
    strcat(colors[3], "#ffd2b7");

    int i;
    for (i = 0; i < 4; i++) {
        fputs("<tr><td bgcolor=\"", myFile);
        fputs(colors[i], myFile);
        fputs("\">b_name</td><td bgcolor=\"", myFile);
        fputs(colors[i], myFile);
        fputs("\" ", myFile);
        if (block->content[i].inodo != -1 && block->content[i].inodo != indexInodo) {
            fputs("port=\"ib", myFile);
            fputs(&to_string(block->content[i].inodo * sizeof(INODO))[0], myFile);
            fputs("\"", myFile);
        }
        fputs(">", myFile);
        fputs(block->content[i].name, myFile);
        fputs("</td></tr>\n", myFile);
        fputs("<tr><td bgcolor=\"", myFile);
        fputs(colors[i], myFile);
        fputs("\">b_inodo</td><td bgcolor=\"", myFile);
        fputs(colors[i], myFile);
        fputs("\">", myFile);
        fputs(&to_string(block->content[i].inodo)[0], myFile);
        fputs("</td></tr>\n", myFile);
    }
    fputs("</table>\n", myFile);
    fputs(">];\n", myFile);
    if (graphConnection) {
        for (i = 2; i < 4; i++) {
            if (block->content[i].inodo != -1) {
                INODO *inodo = readInodo(path, getInitInode(sb, block->content[i].inodo));
                graphInodo(inodo, block->content[i].inodo, myFile, path, sb);
                graphConnectionBloqueInodo(block->content[i].inodo, initBlock, block->content[i].inodo * sizeof(INODO),
                                           myFile);
            }
        }
    }

}

void graphConnectionInodoBloque(int indexnodo, int indexBloque, int indexPuerto, FILE *myFile) {
    fputs("i_", myFile);
    fputs(&to_string(indexnodo)[0], myFile);
    fputs(":", myFile);
    fputs(&to_string(indexPuerto)[0], myFile);
    fputs("->", myFile);
    fputs("b_", myFile);
    fputs(&to_string(indexBloque)[0], myFile);
    fputs(":", myFile);
    fputs(&to_string(indexPuerto)[0], myFile);

    fputs(";\n", myFile);
}

void graphConnectionBloqueBLoque(int indexB1, int indexB2, int indexPuerto, FILE *myFile) {
    fputs("b_", myFile);
    fputs(&to_string(indexB1)[0], myFile);
    fputs(":", myFile);
    fputs(&to_string(indexPuerto)[0], myFile);
    fputs("->", myFile);
    fputs("b_", myFile);
    fputs(&to_string(indexB2)[0], myFile);
    fputs(":", myFile);
    fputs(&to_string(indexPuerto)[0], myFile);
    fputs(";\n", myFile);
}

void graphBlockPointer(int level, int indexPadre, int indexBlock, FILE *fileReport, char path[], SuperBlock *sb,
                       bool graphConnection, int type) {
    BLOCK_POINTER *block = readBlockPointer(path, getInitBlock(sb, indexBlock));
    if (block == NULL) return;
    fputs("b_", fileReport);
    fputs(&to_string(indexBlock)[0], fileReport);
    fputs("[ shape=plaintext label=< \n", fileReport);
    fputs("<table border='0' cellborder='1' cellspacing='0'>\n", fileReport);
    fputs("<tr><td ", fileReport);

    fputs("port=\"", fileReport);
    fputs(&to_string((indexPadre + indexBlock) * sizeof(INODO))[0], fileReport);
    fputs("\"", fileReport);

    fputs(">Bloque ", fileReport);
    fputs(&to_string(indexBlock)[0], fileReport);
    fputs("</td></tr>\n", fileReport);
    int i;
    for (i = 0; i < 15; i++) {
        fputs("<tr><td bgcolor = \"#FFA07A\" ", fileReport);
        if (block->pointers[i] != -1) {
            if (level == 1 && type == 1) {
                fputs("port=\"ib", fileReport);
                fputs(&to_string(block->pointers[i] * sizeof(INODO))[0], fileReport);
                fputs("\"", fileReport);

            } else {
                fputs("port=\"", fileReport);
                int port = (indexBlock + block->pointers[i]) * sizeof(INODO);
                fputs(&to_string(port)[0], fileReport);
                fputs("\"", fileReport);
            }
        }
        fputs(">", fileReport);
        fputs(&to_string(block->pointers[i])[0], fileReport);
        fputs("</td></tr>\n", fileReport);
    }
    fputs("</table>\n", fileReport);
    fputs(">];\n", fileReport);

    for (i = 0; i < 15; i++) {
        if (block->pointers[i] != -1) {
            if (level == 1) {
                if (type == 1) {
                    INODO *ind = readInodo(path, getInitInode(sb, block->pointers[i]));
                    graphInodo(ind, block->pointers[i], fileReport, path, sb);
                    if (graphConnection)
                        graphConnectionBloqueInodo(block->pointers[i], indexBlock, block->pointers[i] * sizeof(INODO),
                                                   fileReport);
                } else {
                    BLOCK_FILE *file = readBlockFile(path, getInitBlock(sb, block->pointers[i]));
                    graphBlockFile(file, block->pointers[i], fileReport, indexBlock);
                    int port = (indexBlock + block->pointers[i]) * sizeof(INODO);
                    if (graphConnection)
                        graphConnectionBloqueBLoque(indexBlock, block->pointers[i], port, fileReport);
                }
            } else {
                graphBlockPointer(level - 1, indexBlock, block->pointers[i], fileReport, path, sb, graphConnection,
                                  type);
                int port = (indexBlock + block->pointers[i]) * sizeof(INODO);
                if (graphConnection)
                    graphConnectionBloqueBLoque(indexBlock, block->pointers[i], port, fileReport);
            }
        }
    }
}

string convertToString(char *a) {
    int i;
    string s = "";
    for (i = 0; i < (sizeof(a) / sizeof(char)); i++) {
        s = s + a[i];
    }
    return s;
}

void reporteDisk(string spath, string id) {
    string path = getPathDisk(id);
    string pathOutput = spath;
//    for (int i = 0; i < path.length(); i++) {
//        path[i] = tolower(spath[i]);
//    }

    string completePath = path;
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
    FILE *fileReport;
    fileReport = fopen("report_disk.dot", "w+");
    if (fileReport == NULL) {
        cout << "Error al crear archivo de reporte\n";
        return;
    }
    fseek(fileReport, 0, SEEK_SET);
    fputs(reporteCompleto.c_str(), fileReport);
    fclose(fileReport);
//    string pathString(path_rep);
    string command = "dot -Tpng report_disk.dot -o \"" + pathOutput + "\"";//+"/report_mbr.png";
    system(command.c_str());

    cout << reporteCompleto << endl;
}

void reporteMBR(string spath, string id) {
    string path = getPathDisk(id);
    string pathOutput = spath;
//    for (int i = 0; i < path.length(); i++) {
//        path[i] = tolower(path[i]);
//    }

    string completePath = path;
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

    string inicioReporte = "digraph test {\n"
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
                          "                        <TR><TD>" + size + "</TD></TR>\n"
                                                                      "                    </TABLE>\n"
                                                                      "                </TD>\n"
                                                                      "            </TR>\n"
                                                                      "            <TR>\n"
                                                                      "                <TD>Fecha creacion MBR</TD>\n"
                                                                      "                <TD>\n"
                                                                      "                    <TABLE BORDER=\"0\">\n"
                                                                      "                        <TR><TD>" +
                          creationDate + "</TD></TR>\n"
                                         "                    </TABLE>\n"
                                         "                </TD>\n"
                                         "            </TR>\n"
                                         "            <TR>\n"
                                         "                <TD>Disk Signature MBR</TD>\n"
                                         "                <TD>\n"
                                         "                    <TABLE BORDER=\"0\">\n"
                                         "                        <TR><TD>" + diskSignature + "</TD></TR>\n"
                                                                                              "                    </TABLE>\n"
                                                                                              "                </TD>\n"
                                                                                              "            </TR>\n"
                                                                                              "            <TR>\n"
                                                                                              "                <TD>Fit Disk</TD>\n"
                                                                                              "                <TD>\n"
                                                                                              "                    <TABLE BORDER=\"0\">\n"
                                                                                              "                        <TR><TD>" +
                          fit + "</TD></TR>\n"
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
            medioReporte += "<TR>\n"
                            "                <TD>Part " + to_string(i) + " Status</TD>\n"
                                                                         "                <TD>\n"
                                                                         "                    <TABLE BORDER=\"0\">\n"
                                                                         "                        <TR><TD>1</TD></TR>\n"
                                                                         "                    </TABLE>\n"
                                                                         "                </TD>\n"
                                                                         "            </TR>\n"
                                                                         "            <TR>\n"
                                                                         "                <TD>Part " + to_string(i) +
                            " Type</TD>\n"
                            "                <TD>\n"
                            "                    <TABLE BORDER=\"0\">\n"
                            "                        <TR><TD>" + type + "</TD></TR>\n"
                                                                        "                    </TABLE>\n"
                                                                        "                </TD>\n"
                                                                        "            </TR>\n"
                                                                        "            <TR>\n"
                                                                        "                <TD>Part " + to_string(i) +
                            " Fit</TD>\n"
                            "                <TD>\n"
                            "                    <TABLE BORDER=\"0\">\n"
                            "                        <TR><TD>" + fitp + "</TD></TR>\n"
                                                                        "                    </TABLE>\n"
                                                                        "                </TD>\n"
                                                                        "            </TR>\n"
                                                                        "             <TR>\n"
                                                                        "                <TD>Part " + to_string(i) +
                            " Size</TD>\n"
                            "                <TD>\n"
                            "                    <TABLE BORDER=\"0\">\n"
                            "                        <TR><TD>" + to_string(sizep) + "</TD></TR>\n"
                                                                                    "                    </TABLE>\n"
                                                                                    "                </TD>\n"
                                                                                    "            </TR>\n"
                                                                                    "             <TR>\n"
                                                                                    "                <TD>Part " +
                            to_string(i) + " Name</TD>\n"
                                           "                <TD>\n"
                                           "                    <TABLE BORDER=\"0\">\n"
                                           "                        <TR><TD>" + name + "</TD></TR>\n"
                                                                                       "                    </TABLE>\n"
                                                                                       "                </TD>\n"
                                                                                       "            </TR>";
        }
    }
    string reporteCompleto = inicioReporte + medioReporte + finalReporte;
    FILE *fileReport;
    fileReport = fopen("report_mbr.dot", "w+");
    if (fileReport == NULL) {
        cout << "Error al crear archivo de reporte\n";
        return;
    }
    fseek(fileReport, 0, SEEK_SET);
    fputs(reporteCompleto.c_str(), fileReport);
    fclose(fileReport);
//    string pathString(path_rep);
    string command = "dot -Tpng report_mbr.dot -o \"" + pathOutput + "\"";//+"/report_mbr.png";
    system(command.c_str());

    cout << reporteCompleto << endl;
}

bool reportBitmap(int rep, string id, char path_rep[]) {
    string pathString = getPathDisk(id);
    char path[pathString.size() + 1];
    strcpy(path, pathString.c_str());
    string nameString = getNamePartition(id);
    char name[nameString.size() + 1];
    strcpy(name, nameString.c_str());
    string completePath = path;
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    SuperBlock *sb = readSuperBlock(sc, name);
    if (sb == NULL) {
        return false;
    }
    FILE *myFile;
    myFile = fopen(sc, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return false;
    }

    FILE *fileReport;
    fileReport = fopen(path_rep, "w+");
    if (fileReport == NULL) {
        cout << "Error al crear archivo de reporte\n";
        return false;
    }
    fseek(fileReport, 0, SEEK_SET);
    if (rep == 0) {
        //reporte de inodos
        char caracter;
        int contador = 0;
        fseek(myFile, sb->bmInodeStart, SEEK_SET);
        while (contador < sb->inodesCount) {
            fread(&caracter, sizeof(char), 1, myFile);
            fwrite(&caracter, sizeof(char), 1, fileReport);
            if ((contador + 1) % 20 == 0 && contador != 0) {
                fwrite("\n", sizeof(char), 1, fileReport);
            }
            contador++;
        }
    } else {
        //reporte de bloques
        char caracter;
        int contador = 0;
        fseek(myFile, sb->bmBlockStart, SEEK_SET);
        while (contador < sb->blocksCount) {
            fread(&caracter, sizeof(char), 1, myFile);
            fwrite(&caracter, sizeof(char), 1, fileReport);
            if ((contador + 1) % 20 == 0 && contador != 0) {
                fwrite("\n", sizeof(char), 1, fileReport);
            }
            contador++;
        }
    }
    fclose(fileReport);
    fclose(myFile);
    delete sb;
    return true;
}

bool reportSuperBlock(string id, char path_rep[]) {
    string pathStringg = getPathDisk(id);
    char path[pathStringg.size() + 1];
    strcpy(path, pathStringg.c_str());
    string nameString = getNamePartition(id);
    char name[nameString.size() + 1];
    strcpy(name, nameString.c_str());
    string completePath = path;
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    SuperBlock *sb = readSuperBlock(sc, name);
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
                         "                        <TR><TD>" + to_string(sb->inodesCount) + "</TD></TR>\n"
                                                                                           "                    </TABLE>\n"
                                                                                           "                </TD>\n"
                                                                                           "            </TR>\n"
                                                                                           "            <TR>\n"
                                                                                           "                <TD>Blocks Count</TD>\n"
                                                                                           "                <TD>\n"
                                                                                           "                    <TABLE BORDER=\"0\">\n"
                                                                                           "                        <TR><TD>" +
                         to_string(sb->blocksCount) + "</TD></TR>\n"
                                                      "                    </TABLE>\n"
                                                      "                </TD>\n"
                                                      "            </TR>\n"
                                                      "           <TR>\n"
                                                      "                <TD>Free Inodes Count</TD>\n"
                                                      "                <TD>\n"
                                                      "                    <TABLE BORDER=\"0\">\n"
                                                      "                        <TR><TD>" +
                         to_string(sb->freeInodesCount) + "</TD></TR>\n"
                                                          "                    </TABLE>\n"
                                                          "                </TD>\n"
                                                          "            </TR>\n"
                                                          "            <TR>\n"
                                                          "                <TD>Free Blocks Count</TD>\n"
                                                          "                <TD>\n"
                                                          "                    <TABLE BORDER=\"0\">\n"
                                                          "                        <TR><TD>" +
                         to_string(sb->freeBlocksCount) + "</TD></TR>\n"
                                                          "                    </TABLE>\n"
                                                          "                </TD>\n"
                                                          "            </TR>\n"
                                                          "            <TR>\n"
                                                          "                <TD>Mtime</TD>\n"
                                                          "                <TD>\n"
                                                          "                    <TABLE BORDER=\"0\">\n"
                                                          "                        <TR><TD>" +
                         asctime(gmtime(&sb->mtime)) + "</TD></TR>\n"
                                                       "                    </TABLE>\n"
                                                       "                </TD>\n"
                                                       "            </TR>\n"
                                                       "            <TR>\n"
                                                       "                <TD>Umtime</TD>\n"
                                                       "                <TD>\n"
                                                       "                    <TABLE BORDER=\"0\">\n"
                                                       "                        <TR><TD>" +
                         asctime(gmtime(&sb->umtime)) + "</TD></TR>\n"
                                                        "                    </TABLE>\n"
                                                        "                </TD>\n"
                                                        "            </TR>\n"
                                                        "            <TR>\n"
                                                        "                <TD>Magic</TD>\n"
                                                        "                <TD>\n"
                                                        "                    <TABLE BORDER=\"0\">\n"
                                                        "                        <TR><TD>" + to_string(sb->magic) +
                         "</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Inode Size</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>" + to_string(sb->inodeSize) + "</TD></TR>\n"
                                                                                         "                    </TABLE>\n"
                                                                                         "                </TD>\n"
                                                                                         "            </TR>\n"
                                                                                         "            <TR>\n"
                                                                                         "                <TD>Block Size</TD>\n"
                                                                                         "                <TD>\n"
                                                                                         "                    <TABLE BORDER=\"0\">\n"
                                                                                         "                        <TR><TD>" +
                         to_string(sb->blockSize) + "</TD></TR>\n"
                                                    "                    </TABLE>\n"
                                                    "                </TD>\n"
                                                    "            </TR>\n"
                                                    "            <TR>\n"
                                                    "                <TD>First Inode</TD>\n"
                                                    "                <TD>\n"
                                                    "                    <TABLE BORDER=\"0\">\n"
                                                    "                        <TR><TD>" + to_string(sb->firstInode) +
                         "</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Frist Block</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>" + to_string(sb->firstBlock) + "</TD></TR>\n"
                                                                                          "                    </TABLE>\n"
                                                                                          "                </TD>\n"
                                                                                          "            </TR>\n"
                                                                                          "            <TR>\n"
                                                                                          "                <TD>BM Inode Start</TD>\n"
                                                                                          "                <TD>\n"
                                                                                          "                    <TABLE BORDER=\"0\">\n"
                                                                                          "                        <TR><TD>" +
                         to_string(sb->bmInodeStart) + "</TD></TR>\n"
                                                       "                    </TABLE>\n"
                                                       "                </TD>\n"
                                                       "            </TR>\n"
                                                       "            <TR>\n"
                                                       "                <TD>BM Block Start</TD>\n"
                                                       "                <TD>\n"
                                                       "                    <TABLE BORDER=\"0\">\n"
                                                       "                        <TR><TD>" +
                         to_string(sb->bmBlockStart) + "</TD></TR>\n"
                                                       "                    </TABLE>\n"
                                                       "                </TD>\n"
                                                       "            </TR>\n"
                                                       "            <TR>\n"
                                                       "                <TD>Inode Start</TD>\n"
                                                       "                <TD>\n"
                                                       "                    <TABLE BORDER=\"0\">\n"
                                                       "                        <TR><TD>" + to_string(sb->inodeStart) +
                         "</TD></TR>\n"
                         "                    </TABLE>\n"
                         "                </TD>\n"
                         "            </TR>\n"
                         "            <TR>\n"
                         "                <TD>Block Start</TD>\n"
                         "                <TD>\n"
                         "                    <TABLE BORDER=\"0\">\n"
                         "                        <TR><TD>" + to_string(sb->blockStart) + "</TD></TR>\n"
                                                                                          "                    </TABLE>\n"
                                                                                          "                </TD>\n"
                                                                                          "            </TR>\n"
                                                                                          "        </TABLE>\n"
                                                                                          "    >, ];\n"
                                                                                          "}";
    FILE *fileReport;
    fileReport = fopen("report_sb.dot", "w+");
    if (fileReport == NULL) {
        cout << "Error al crear archivo de reporte\n";
        return false;
    }
    fseek(fileReport, 0, SEEK_SET);
    fputs(reporteBody.c_str(), fileReport);
    fclose(fileReport);
    string pathString(path_rep);
    string command = "dot -Tpng report_sb.dot -o \"" + pathString + "\"";//+"/report_mbr.png";
    system(command.c_str());
    cout << reporteBody << endl;
    return true;
}

bool reportInodes(string id, char path_report[]) {
    string pathStringg = getPathDisk(id);
    char path[pathStringg.size() + 1];
    strcpy(path, pathStringg.c_str());
    string nameString = getNamePartition(id);
    char name[nameString.size() + 1];
    strcpy(name, nameString.c_str());
    SuperBlock *sb = readSuperBlock(path, name);
    if (sb == NULL) {
        return false;
    }
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return false;
    }

    FILE *fileReport;
    fileReport = fopen("report_inodes.dot", "w+");
    if (fileReport == NULL) {
        cout << "Error al crear archivo de reporte\n";
        return false;
    }
    fseek(fileReport, 0, SEEK_SET);
    fputs("digraph {\n", fileReport);
    //reporte de inodos
    char caracter;
    int contador = 0;
    INODO *inodo;
    fseek(myFile, sb->bmInodeStart, SEEK_SET);
    while (contador < sb->inodesCount) {
        fread(&caracter, sizeof(char), 1, myFile);
        //******
        if (caracter == '1') {
            inodo = readInodo(path, getInitInode(sb, contador));
            if (inodo == NULL) {
                break;
            }
            fputs("i_", fileReport);
            fputs(&to_string(contador)[0], fileReport);
            fputs("[ shape=plaintext label=< \n", fileReport);
            fputs("<table border=\"1\" cellborder = \"0\" cellspacing='0'>\n", fileReport);
            fputs("<tr><td colspan=\"3\">Inodo ", fileReport);
            fputs(&to_string(contador)[0], fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_uid</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(&to_string(inodo->uid)[0], fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_gid</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(&to_string(inodo->gid)[0], fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_size</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(&to_string(inodo->size)[0], fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_atime</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(asctime(gmtime(&inodo->atime)), fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_ctime</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(asctime(gmtime(&inodo->ctime)), fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_mtime</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(asctime(gmtime(&inodo->mtime)), fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_type</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(&to_string(inodo->type)[0], fileReport);
            fputs("</td></tr>\n", fileReport);
            fputs("<tr><td bgcolor=\"#DAF7A6\">i_perm</td><td bgcolor=\"#DAF7A6\">", fileReport);
            fputs(&to_string(inodo->permisos)[0], fileReport);
            fputs("</td></tr>\n", fileReport);
            int i = 0;
            //APUNTADORES DIRECTOS
            while (i < 12) {
                fputs("<tr><td bgcolor=\"#FFC300\">AD", fileReport);
                fputs(&to_string(i + 1)[0], fileReport);
                fputs("</td><td ", fileReport);
                fputs(" bgcolor=\"#FFC300\">", fileReport);
                fputs(&to_string(inodo->block[i])[0], fileReport);
                fputs("</td></tr>\n", fileReport);
                i++;
            }

            while (i < 15) {
                fputs("<tr><td bgcolor=\"#FF5733\">AI", fileReport);
                fputs(&to_string(i - 11)[0], fileReport);
                fputs("</td><td bgcolor=\"#FF5733\">", fileReport);
                fputs(&to_string(inodo->block[i])[0], fileReport);
                fputs("</td></tr>\n", fileReport);
                i++;
            }

            fputs("</table>\n", fileReport);
            fputs(">];\n", fileReport);
        }
        //******
        contador++;
    }
    fputs("}\n", fileReport);
    fclose(fileReport);
    fclose(myFile);
    string pathString(path_report);
    string command = "dot -Tpng report_inodes.dot -o \"" + pathString + "\"";//+"/report_mbr.png";
    system(command.c_str());
    return true;
}

bool reportBlocks(string id, char path_report[]) {
    string pathStringg = getPathDisk(id);
    char path[pathStringg.size() + 1];
    strcpy(path, pathStringg.c_str());
    string nameString = getNamePartition(id);
    char name[nameString.size() + 1];
    strcpy(name, nameString.c_str());

    SuperBlock *sb = readSuperBlock(path, name);
    if (sb == NULL) {
        return false;
    }
    FILE *myFile;
    myFile = fopen(path, "rb+");
    if (myFile == NULL) {
        cout << "Error al abrir el disco\n";
        return false;
    }

    FILE *fileReport;
    fileReport = fopen("report_blocks.dot", "w");
    if (fileReport == NULL) {
        cout << "Error al crear archivo de reporte\n";
        return false;
    }
    fseek(fileReport, 0, SEEK_SET);
    fputs("digraph di{\n", fileReport);
    //reporte de inodos
    char caracter;
    int contador = 0;
    INODO *inodo;
    fseek(myFile, sb->bmInodeStart, SEEK_SET);
    while (contador < sb->inodesCount) {
        fread(&caracter, sizeof(char), 1, myFile);
        if (caracter == '1') {
            inodo = readInodo(path, getInitInode(sb, contador));
            if (inodo == NULL) {
                break;
            }
            int i;
            //apuntadores directos
            for (i = 0; i < 12; i++) {
                if (inodo->block[i] != -1) {
                    if (inodo->type == 1) {
                        BLOCK_DIR *block = readBlockDirectory(path, getInitBlock(sb, inodo->block[i]));
                        graphBlockDirectory(block, inodo->block[i], fileReport, contador, sb, path, false);
                        //******
                    } else if (inodo->type == 0) {
                        BLOCK_FILE *block = readBlockFile(path, getInitBlock(sb, inodo->block[i]));
                        graphBlockFile(block, inodo->block[i], fileReport, contador);
                    }
                }
            }
            //apuntadores indirectos
            for (i = 12; i < 15; i++) {
                if (inodo->block[i] != -1) {
                    graphBlockPointer(i - 11, contador, inodo->block[i], fileReport, path, sb, false, inodo->type);
                }
            }
        }
        contador++;
    }
    fputs("}\n", fileReport);
    fclose(fileReport);
    fclose(myFile);
    string pathString(path_report);
    string command = "dot -Tpng report_blocks.dot -o \"" + pathString + "\"";//+"/report_mbr.png";
    system(command.c_str());
    return true;
}


bool reportFile() { return true; }

bool reportTree() { return true; }

struct ParamsReport {
    string name;
    string path;
    string id;
};

ParamsReport separateParamsReports(string params[5]) {
    ParamsReport paramsReport;
    for (int i = 0; i < 5; ++i) {
        if (params[i][0] == '1' || params[i][0] == '2' || params[i][0] == '3' || params[i][0] == '4' ||
            params[i][0] == '5' || params[i][0] == '6' || params[i][0] == '7' || params[i][0] == '8' ||
            params[i][0] == '9' || params[i][0] == '0') {
            paramsReport.id = params[i];
        } else if (params[i][0] == '/' || params[i][0] == '"') {
            paramsReport.path = params[i];
        } else {
            if (params[i].size() != 0) {
                paramsReport.name = params[i];
            }
        }
    }
    return paramsReport;
}

void reportTree(char path_report[], char id[]) {
    string pathDisk = getPathDisk(id);
    string namePartition = getNamePartition(id);

    char pathRead[pathDisk.size() + 1];
    strcpy(pathRead, pathDisk.c_str());
    char nameRead[namePartition.size() + 1];
    strcpy(nameRead, namePartition.c_str());
    SuperBlock *sb = readSuperBlock(pathRead, nameRead);
    if (sb == NULL) {
        return;
    }

    FILE *myFile;
    myFile = fopen("report_tree.dot", "w+");
    if (myFile == NULL) {
        cout << "Error al crear el archivo de reporte\n";
        return;
    }
    fseek(myFile, 0, SEEK_SET);
    fputs("digraph G {\n", myFile);
    fputs("rankdir =LR;\n", myFile);
    //graphAllInodes(sb,myFile,disk->path);
    INODO *inodo = readInodo(pathRead, getInitInode(sb, 0));
    if (inodo != NULL) {
        graphInodo(inodo, 0, myFile, pathRead, sb);
    }
    fputs("}\n", myFile);
    //cerrando stream
    fclose(myFile);
    string pathString(path_report);
    string command = "dot -Tpng report_tree.dot -o \"" + pathString + "\"";//+"/report_disk.png";
    system(command.c_str());
}

void makeReports(string params[5]) {
    ParamsReport paramsUtiles = separateParamsReports(params);
    if (paramsUtiles.name == "mbr") {
        reporteMBR(paramsUtiles.path, paramsUtiles.id);
    } else if (paramsUtiles.name == "disk") {
        reporteDisk(paramsUtiles.path, paramsUtiles.id);
    } else if (paramsUtiles.name == "inode") {
        char pathreport[paramsUtiles.path.size() + 1];
        strcpy(pathreport, paramsUtiles.path.c_str());
        reportInodes(paramsUtiles.id, pathreport);
    } else if (paramsUtiles.name == "block") {
        char pathreport[paramsUtiles.path.size() + 1];
        strcpy(pathreport, paramsUtiles.path.c_str());
        reportBlocks(paramsUtiles.id, pathreport);
    } else if (paramsUtiles.name == "sb") {
        char pathreport[paramsUtiles.path.size() + 1];
        strcpy(pathreport, paramsUtiles.path.c_str());
        reportSuperBlock(paramsUtiles.id, pathreport);
    } else if (paramsUtiles.name == "bm_inode") {
        char pathreport[paramsUtiles.path.size() + 1];
        strcpy(pathreport, paramsUtiles.path.c_str());
        reportBitmap(0, paramsUtiles.id, pathreport);
    } else if (paramsUtiles.name == "bm_block") {
        char pathreport[paramsUtiles.path.size() + 1];
        strcpy(pathreport, paramsUtiles.path.c_str());
        reportBitmap(1, paramsUtiles.id, pathreport);
    } else if (paramsUtiles.name == "tree") {
        char pathreport[paramsUtiles.path.size() + 1];
        strcpy(pathreport, paramsUtiles.path.c_str());
        char idreport[paramsUtiles.id.size() + 1];
        strcpy(idreport, paramsUtiles.id.c_str());
        reportTree(pathreport, idreport);
    }
}


//todo hacer el reporte con los ebr y mostrarlos en el reporte de disco