#include <iostream>
#include "mkdisk.h"
using namespace std;

ParamsFDisk separarParams(string params[8])
{
    ParamsFDisk paramsFinal;
    string datos[8];
    for (int i = 0; i < 8; i++)
    {
        datos[i] = params[i];
        for (int j = 0; j < params[i].length(); j++)
        {
            datos[i][j] = tolower(params[i][j]);
        }

        if (datos[i][0] == '0' || datos[i][0] == '1' || datos[i][0] == '2' || datos[i][0] == '3' || datos[i][0] == '4' || datos[i][0] == '5' || datos[i][0] == '6' || datos[i][0] == '7' || datos[i][0] == '8' || datos[i][0] == '9')
        {
            paramsFinal.size = stoi(datos[i]);
        }
        else if ((datos[i][0] == 'b' || datos[i][0] == 'k' || datos[i][0] == 'm') && datos[i].length() == 1)
        {
            paramsFinal.units = datos[i][0];
        }
        else if (datos[i][0] == '"')
        {
            if (datos[i][1] == '/')
            {
                paramsFinal.path = datos[i];
            }
            else
            {
                paramsFinal.name = datos[i];
            }
        }
        else if (datos[i][0] == '/')
        {
            paramsFinal.path = datos[i];
        }
        else if ((datos[i][0] == 'p' || datos[i][0] == 'e' || datos[i][0] == 'l') && datos[i].length() == 1)
        {
            paramsFinal.type = datos[i][0];
        }
        else if (datos[i] == "bf" || datos[i] == "ff" || datos[i] == "wf")
        {
            paramsFinal.fit = datos[i];
        }
        else if (datos[i][0] == '%')
        {
            paramsFinal.add = stoi(datos[i].substr(1, datos[i].length() - 1));
        }
        else if (datos[i] == "fast" || datos[i] == "full")
        {
            paramsFinal.del = datos[i];
        }
        else if (datos[i][0] == '#')
        {
            paramsFinal.name = datos[i].substr(1, datos[i].length() - 1);
        }
    }
    return paramsFinal;
}

void makePartition(ParamsFDisk params)
{
    int size = params.size;
    char units = params.units;
    string path = params.path;
    char type = params.type;
    string fit = params.fit;
    string del = params.del;
    string name = params.name;
    int add = params.add;
    if (units == 'b')
    {
        size = params.size;
    }
    else if (units == 'k')
    {
        size = params.size * 1024;
    }
    else if (units == 'm')
    {
        size = params.size * 1024 * 1024;
    }
    else
    {
        size = params.size * 1024;
    }

    string completePath = rpath + path;
    if (!exist_file(completePath))
    {
        cout << "disco no encontrado" << endl;
        cout << "revisa la ruta: " << completePath << endl;
    }
    char sc[completePath.size() + 1];
    strcpy(sc, completePath.c_str());
    FILE *file = fopen(sc, "rb+");
    fseek(file, 0, SEEK_SET);
    MBR mbr;
    fread(&mbr, sizeof(MBR), 1, file);
    cout << "------------------------------------------------disco encontrado------------------------------------------------" << endl;
    cout << "Fecha creacion: " << asctime(gmtime(&mbr.creationDate)) << endl;
    cout << "Signature: " << mbr.diskSignature << endl;
    cout << "Tamano: " << mbr.size << endl;
    cout << "Fit: " << mbr.fit << endl;
    cout << "inicio 1 " << mbr.partition[1].name << endl;

    int index = 0;
    bool existente = false;
    for (int i = 0; i < 4; i++)
    {
        if (mbr.partition[i].name == name)
        {
            existente = true;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (add == 0 && del == "")
        {
            if (mbr.partition[i].status == '0' && !existente)
            {
                index = i;
                break;
            }
        }
        else
        {
            if (mbr.partition[i].name == name)
            {
                index = i;
                break;
            }
        }
    }
    if (add == 0 && del == "")
    {
        if (mbr.partition[index].status == '0')
        {
            mbr.partition[index].status = '1';
            mbr.partition[index].size = size;
            if (fit == "bf")
            {
                mbr.partition[index].fit = 'b';
            }
            else if (fit == "wf")
            {
                mbr.partition[index].fit = 'w';
            }
            else
            {
                mbr.partition[index].fit = 'f';
            }
            //todo tengo que ver como poner el start
            if (mbr.partition[index].fit == 'f')
            {
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        int posible = mbr.partition[j].start - (mbr.partition[i].start + mbr.partition[i].size);
                        if (posible > mbr.partition[index].size)
                        {
                            mbr.partition[index].start = mbr.partition[i].start + mbr.partition[i].size + 1;
                        }
                    }
                }
            }
            strcpy(mbr.partition[index].name, name.c_str());
            cout << "particion creada con exito" << endl;
        }
        else
        {
            cout << "no se puede crear una particion" << endl;
            cout << mbr.partition[index].status << endl;
            cout << mbr.partition[index].start << endl;
            cout << mbr.partition[index].size << endl;
            cout << mbr.partition[index].fit << endl;
        }
    }
    else if (del != "")
    {
    }
    else if (add != 0)
    {
    }
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, file);
    fclose(file);
}
