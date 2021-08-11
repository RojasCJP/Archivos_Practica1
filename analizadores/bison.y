%{
#include <stdio.h>
#include <iostream>
#include <array>

using namespace std;

std::array<std::string, 11> getDatos();
void cleanEs();
int Pause();
int yylex();
int yyerror(const char* msg){
        cout<< "Syntax Error" << msg << endl;
        return 0;
}
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
%token <entrance> size
%token <entrance> fit
%token <entrance> units
%token <entrance> igual
%token <text> path
%token <other> type
%token <other> name
%token <other> add
%token <other> id
%token <other> delet
%token <other> format
%token <other> readfile

%token <number> number
%token <text> e_path
%token <text> e_name
%token <entrance> e_fit
%token <entrance> e_units
%token <entrance> e_type
%token <entrance> e_delet
%token <entrance> e_id


%start INICIO
%union{
    int number;
    char text[200];
    char entrance[12];
    int other;
}

%%

INICIO: F_MKDISK
;

F_MKDISK: mkdisk PARAMSMK
;

PARAMSMK: PARAMMK PARAMSMK
        |PARAMMK
;

PARAMMK: path igual e_path
        |fit igual e_fit
        |units igual e_units
        |size igual number
;

%%
