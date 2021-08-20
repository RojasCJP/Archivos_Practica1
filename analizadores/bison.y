%{
#include <stdio.h>
#include <iostream>
#include <array>
#include <list>
#include <cstring>
#include "../comandos/mkdisk.h"
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
int getIndex(string params[4]);
void inicializarArreglo(string params[4]);
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
;

F_MKDISK: mkdisk PARAMSMK {cout<<"abr "<<parametros[0]<<parametros[1]<<parametros[2]<<parametros[3]<<endl;
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
RemoveDisk($4);
}
;

F_FDISK: fdisk PARAMSF
;

PARAMSF: PARAMF PARAMSF
        |PARAMF
;

PARAMF: size igual number
        |units igual e_units
        |path igual e_path
        |type igual e_type
        |fit igual e_fit
        |delet igual e_delet
        |name igual e_name
        |add igual number
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

F_UNMOUNT: unmount id igual e_id
;

F_MKFS: mkfs PARAMSMKFS
;

PARAMSMKFS: PARAMMKFS PARAMSMKFS
        |PARAMMKFS
;

PARAMMKFS: id igual e_id
        |type igual e_type
        |fs igual e_name
;

F_LOGIN: login PARAMSLOGIN
;

PARAMSLOGIN: PARAMLOGIN PARAMSLOGIN
        |PARAMLOGIN
;

PARAMLOGIN: usr igual e_name
        |pass igual password
        |id igual e_id
;

F_LOGOUT: logout
;

F_MKGRP: mkgrp name igual e_name
;

F_RMGRP: rmgrp name igual e_name
;

F_MKUSR: mkusr PARAMSMKUSR
;

PARAMSMKUSR: PARAMMKUSR PARAMSMKUSR
        |PARAMMKUSR
;

PARAMMKUSR: usr igual e_name
        |pass igual password
        |grp igual e_name
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

F_TOUCH: touch PARAMSTOUCH;

PARAMSTOUCH: PARAMSTOUCH PARAMTOUCH
        |PARAMTOUCH
;

PARAMTOUCH: path igual e_path
        |recursive
        |size igual number
        |cont igual e_path
        |stdinn
;

F_CAT: cat filen igual e_path
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

F_MKDIR: mkdir PARAMSMKDIR
;

PARAMSMKDIR: PARAMSMKDIR PARAMMKDIR
        |PARAMMKDIR
;

PARAMMKDIR: path igual e_path
        |pp
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