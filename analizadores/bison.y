%{
#include <stdio.h>
#include <iostream>
#include <array>
#include <list>
#include <cstring>
#include "../comandos/reportes.h"
#define MAX_DIGITS 10

using namespace std;
std::array<std::string, 11> getDatos();
void cleanEs();
int Pause();
int yylex();
int yyerror(const char* msg){
        cout<< "Syntax Error" << msg << endl;
        return 0;
}
std::string parametros[4];
std::string parametros8[8];
std::string parametros3[3];
std::string parametros5[5];
int getIndex(string params[4]);
void inicializarArreglo(string params[4]);
int getIndex8(string params[8]);
void inicializarArreglo8(string params[8]);
int getIndex3(string params[3]);
void inicializarArreglo3(string params[3]);
int getIndex5(string params[5]);
void inicializarArreglo5(string params[5]);
%}

%define parse.error verbose
%token <number> show
%token <number> pausa
%token <text> comentario //este creo que lo voy a tener que quitar

%token <text> mkdisk
%token <text> rmdisk
%token <text> fdisk
%token <text> mount
%token <text> unmount
%token <text> mkfs
%token <text> rep
%token <text> ruta
%token <text> root
%token <text> chmod
%token <text> touch
%token <text> cat
%token <text> rm
%token <text> edit
%token <text> ren
%token <text> rem
%token <text> findd
%token <text> mkdir
%token <text> cp
%token <text> mv
%token <text> mkfile
%token <text> chownn
%token <text> chgrp
%token <entrance> size
%token <entrance> fit
%token <entrance> units
%token <entrance> igual
%token <entrance> grp
%token <entrance> ugo
%token <entrance> recursive
%token <entrance> cont
%token <entrance> stdinn
%token <entrance> filen
%token <entrance> pp
%token <entrance> dest
%token <text> path
%token <other> type
%token <other> name
%token <other> add
%token <other> id
%token <other> delet
%token <other> format
%token <other> readfile
%token <other> fs

%token <number> number
%token <text> e_path
%token <text> password
%token <entrance> e_fit
%token <entrance> e_units
%token <entrance> e_type
%token <entrance> e_delet
%token <text> e_name
%token <entrance> e_id

%token <text> login
%token <text> logout
%token <text> pass
%token <text> usr

%token <text> mkgrp
%token <text> rmgrp
%token <text> mkusr
%token <text> rmusr

%type <text> PARAMMK
%type <text> PARAMMOUNT
%type <text> PARAMF
%type <text> PARAMMKFS
%type <text> PARAMREP
%type <text> PARAMLOGIN
%type <text> PARAMMKUSR
%type <text> PARAMTOUCH

%start START
%union{
    int number;
    char text[200];
    char entrance[12];
    int other;
}

%%

START: START INICIO
        |INICIO
;

INICIO: F_MKDISK
        |F_RMDISK
        |F_FDISK
        |F_MOUNT
        |F_UNMOUNT
        |F_MKFS
        |F_LOGIN
        |F_LOGOUT
        |F_MKGRP
        |F_RMGRP
        |F_MKUSR
        |F_RMUSR
        |F_CHMOD
        |F_TOUCH
        |F_CAT
        |F_RM
        |F_EDIT
        |F_REN
        |F_MKDIR
        |F_CP
        |F_MV
        |F_REP
;

F_MKDISK: mkdisk PARAMSMK {
CreateDisk(diskConstructor(parametros));
inicializarArreglo(parametros); 
}
;

PARAMSMK: PARAMMK PARAMSMK {parametros[getIndex(parametros)]=$1;}
        |PARAMMK { parametros[getIndex(parametros)]=$1;}
;

PARAMMK: path igual e_path {strcpy($$, $3);}
        |fit igual e_fit {strcpy($$, $3);}
        |units igual e_units {strcpy($$, $3);}
        |size igual number {char num_char[MAX_DIGITS + sizeof(char)];sprintf(num_char, "%d", $3);strcpy($$, num_char);}
;

F_RMDISK: rmdisk path igual e_path{
RemoveDisk($4);}
;

F_FDISK: fdisk PARAMSF{makePartition(separarParams(parametros8));
inicializarArreglo8(parametros8);}
;

PARAMSF: PARAMF PARAMSF {parametros8[getIndex8(parametros8)]=$1;}
        |PARAMF {parametros8[getIndex8(parametros8)]=$1;}
;

PARAMF: size igual number {char num_char[MAX_DIGITS + sizeof(char)];sprintf(num_char, "%d", $3);strcpy($$, num_char);}
        |units igual e_units {strcpy($$, $3);}
        |path igual e_path {strcpy($$, $3);}
        |type igual e_type {strcpy($$, $3);}
        |fit igual e_fit {strcpy($$, $3);}
        |delet igual e_delet {strcpy($$, $3);}
        |name igual e_name {string name = $3;string nameComplete = '#'+name; strcpy($$,nameComplete.c_str());}
        |add igual number {char num_char[MAX_DIGITS + sizeof(char)];sprintf(num_char, "%d", $3); string numchar2 = num_char; string numchar3 = '%'+numchar2; strcpy($$, numchar3.c_str()); }
;

F_MOUNT: mount PARAMSMOUNT {Mount(getPathMount(parametros), getNameMount(parametros));
inicializarArreglo(parametros);}
;

PARAMSMOUNT: PARAMMOUNT PARAMSMOUNT { parametros[getIndex(parametros)]=$1;}
        |PARAMMOUNT { parametros[getIndex(parametros)]=$1;}
;

PARAMMOUNT: path igual e_path {strcpy($$, $3);}
        |name igual e_name {strcpy($$, $3);}
;

F_UNMOUNT: unmount id igual e_id {Unmount($4);}
;

F_MKFS: mkfs PARAMSMKFS {mkfsMethod(separarParamsMKFS(parametros3));
inicializarArreglo3(parametros3);}
;

PARAMSMKFS: PARAMMKFS PARAMSMKFS {parametros3[getIndex3(parametros3)]=$1;}
        |PARAMMKFS {parametros3[getIndex3(parametros3)]=$1;}
;

PARAMMKFS: id igual e_id {strcpy($$, $3);}
        |type igual e_delet {strcpy($$, $3);}
        |fs igual password {strcpy($$, $3);}
;

F_REP: rep PARAMSREP {makeReports(parametros5);
inicializarArreglo5(parametros5);}
;

PARAMSREP: PARAMREP PARAMSREP {parametros5[getIndex5(parametros5)]=$1;}
            |PARAMREP {parametros5[getIndex5(parametros5)]=$1;}
;

PARAMREP: name igual e_name {strcpy($$, $3);}
         |path igual e_path {strcpy($$, $3);}
         |id igual e_id {strcpy($$, $3);}
         |ruta igual e_path {strcpy($$, $3);}
         |root {strcpy($$, $1);}
;

F_LOGIN: login PARAMSLOGIN {loginn(separarParamsLogin(parametros3).user,separarParamsLogin(parametros3).pass,separarParamsLogin(parametros3).id);
inicializarArreglo3(parametros3);}
;

PARAMSLOGIN: PARAMLOGIN PARAMSLOGIN {parametros3[getIndex3(parametros3)]=$1;}
        |PARAMLOGIN {parametros3[getIndex3(parametros3)]=$1;}
;

PARAMLOGIN: usr igual e_name {string name = $3;string nameComplete = '#'+name; strcpy($$,nameComplete.c_str());}
        |pass igual password {strcpy($$, $3);}
        |pass igual e_name {strcpy($$, $3);}
        |pass igual e_id {strcpy($$, $3);}
        |pass igual number {char num_char[MAX_DIGITS + sizeof(char)];sprintf(num_char, "%d", $3);strcpy($$, num_char);}
        |id igual e_id {strcpy($$, $3);}
;

F_LOGOUT: logout
;

F_MKGRP: mkgrp name igual e_name {addGroup(active_sesion->id,$4,false);}
;

F_RMGRP: rmgrp name igual e_name
;

F_MKUSR: mkusr PARAMSMKUSR {
UsrParams usuario = separarUsrParams(parametros3);
addUser(active_sesion->id,usuario.user,usuario.pass,usuario.group,false);
inicializarArreglo3(parametros3);}
;

PARAMSMKUSR: PARAMMKUSR PARAMSMKUSR {parametros3[getIndex3(parametros3)]=$1;}
        |PARAMMKUSR {parametros3[getIndex3(parametros3)]=$1;}
;

PARAMMKUSR: usr igual e_name {string name = $3;string nameComplete = '#'+name; strcpy($$,nameComplete.c_str());}
        |pass igual e_name {string name = $3;string nameComplete = '%'+name; strcpy($$,nameComplete.c_str());}
        |pass igual e_id {string name = $3;string nameComplete = '%'+name; strcpy($$,nameComplete.c_str());}
        |pass igual number {char num_char[MAX_DIGITS + sizeof(char)];sprintf(num_char, "%d", $3); string numchar2 = num_char; string numchar3 = '%'+numchar2; strcpy($$, numchar3.c_str()); }
        |pass igual password {string name = $3;string nameComplete = '%'+name; strcpy($$,nameComplete.c_str());}
        |grp igual e_name {strcpy($$, $3);}
;

F_RMUSR: rmusr usr igual e_name
;

F_CHMOD: chmod PARAMSCHMOD
;

PARAMSCHMOD: PARAMCHMOD PARAMSCHMOD
        |PARAMCHMOD
;

PARAMCHMOD: path igual e_path
        |recursive
        |ugo igual number
;

F_TOUCH: touch PARAMSTOUCH {
    ParamsCreateFile uses = CreateFilesSeparar(parametros);
    createFile(uses.path,uses.r,uses.size,false);
    editFile(uses.path,uses.contenido,false);
};

PARAMSTOUCH: PARAMTOUCH PARAMSTOUCH {parametros[getIndex(parametros)]=$1;}
        |PARAMTOUCH {parametros[getIndex(parametros)]=$1;}
;

PARAMTOUCH: path igual e_path {strcpy($$, $3);}
        |recursive { strcpy($$ , "re");}
        |size igual number {char num_char[MAX_DIGITS + sizeof(char)];sprintf(num_char, "%d", $3);strcpy($$, num_char);}
        |stdinn { strcpy($$ ,"#a");}
;

F_CAT: cat filen igual e_path {catFile($4, active_sesion->path,active_sesion->namePartition);}
;

F_RM: rm path igual e_path
;

F_EDIT: edit PARAMSEDIT
;

PARAMSEDIT: PARAMSEDIT PARAMEDIT
        |PARAMEDIT
;

PARAMEDIT: path igual e_path
        |cont igual e_path
        |stdinn
;

F_REN: ren PARAMSREN
;

PARAMSREN: PARAMSREN PARAMREN
        |PARAMREN
;

PARAMREN: path igual e_path
        |name igual e_name
;

F_MKDIR: mkdir path igual e_path pp {createDirectory(true,active_sesion->id,$4,false);}
        | mkdir pp path igual e_path {createDirectory(true,active_sesion->id,$5,false);}
;

F_CP: cp PARAMSCP
;

PARAMSCP: PARAMSCP PARAMCP
        |PARAMCP
;

PARAMCP: path igual e_path
        |dest igual e_path
;

F_MV: mv PARAMSCP
;

%%

int getIndex(string params[4]){
        for(int i = 0 ; i<4;i++){
                if(params[i]==""){
                        return i;
                }
        }
        return 0;
}

void inicializarArreglo(string params[4]){
        for(int i = 0; i<4; i++){
                params[i] = "";
        }
}

int getIndex8(string params[8]){
        for(int i = 0; i<8; i++){
                if(params[i]==""){
                        return i;
                }
        }
        return 0;
}

void inicializarArreglo8(string params[8]){
        for(int i = 0; i<8;i++){
                params[i]="";
        }
}

int getIndex3(string params[3]){
        for(int i = 0; i<3; i++){
                if(params[i]==""){
                        return i;
                }
        }
        return 0;
}

void inicializarArreglo3(string params[3]){
        for(int i = 0; i<3;i++){
                params[i]="";
        }
}

int getIndex5(string params[5]){
        for(int i = 0; i<5; i++){
                if(params[i]==""){
                        return i;
                }
        }
        return 0;
}

void inicializarArreglo5(string params[5]){
        for(int i = 0; i<5;i++){
                params[i]="";
        }
}