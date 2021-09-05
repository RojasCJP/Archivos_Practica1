%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <iostream>
    #include "parser.h"

    using namespace std;
%}

%option case-insensitive
%option noyywrap

DIGITO [0-9]
LETRA [a-z]
ENTERO {DIGITO}+
REAL {DIGITO}+"."{DIGITO}+
MENOS "-"
NEGATIVO {MENOS}({REAL}|{ENTERO})
IDENTIFICADOR {LETRA}+

MKDISK      "mkdisk"
RMDISK      "rmdisk"
FDISK       "fdisk"
MOUNT       "mount"
UNMOUNT     "unmount"
MKFS        "mkfs"
RM          "rm"

LOGIN       "login"
LOGOUT      "logout"
MKGRP       "mkgrp"
RMGRP       "rmgrp"
MKUSR       "mkusr"
RMUSR       "rmusr"

CHMOD       "chmod"
MKFILE      "mkfile"
TOUCH       "touch"
CAT         "cat"
REM         "rem"
EDIT        "edit"
REN         "ren"
MKDIR       "mkdir"
CP          "cp"
MV          "mv"
FIND        "find"
CHOWN       "chown"
CHGRP       "chgrp"
PAUSE       "pause"
REP         "rep"

SIZE        "-size"
FIT         "-f"
UNITS       "-u"
PATH        "-path"
RUTA        "-ruta"
ROOT        "-root"
TYPE        "-type"
DELET       "-delete"
NAME        "-name"
ADD         "-add"
USER        "-usr"
PASS        "-pwd"
ID          "-id"
FORMAT      "-fs"
GROUP       "-grp"
RECURSIVE   "-r"
UGO         "-ugo"
CONT        "-cont"
STDIN       "-stdin"
FILEN       "-filen"
PP          "-p"
DEST        "-dest"

IGUAL       "="
SLASH       "/"
SEMIROUTE   {IDENTIFICADOR}({IDENTIFICADOR}|{DIGITO})*{SLASH}

BF          "bf"
FF          "ff"
WF          "wf"
KILOBY      "k"
MEGBY       "m"
BYTE        "b"
E_P         "p"
E_E         "e"
E_L         "l"
FAST        "fast"
FULL        "full"
PATH1       {SLASH}{SEMIROUTE}*{NAME_FILE}
PATH2       \"{SLASH}({IDENTIFICADOR}|{ENTERO}|{SLASH}|" ")+{NAME_FILE}\"
PASSWORD    ({IDENTIFICADOR}|{ENTERO})+
NAME1       {IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO})*
NAME2       \"{IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO}|" ")*\"
IDES        {ENTERO}+[a-zA-Z]


DISK_EXTENTION1 "dk"
DISK_EXTENTION2 "disk"
SCRIPT_EXTENTION "script" 
NAME_FILE       {IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO})*"."({DISK_EXTENTION2}|{DISK_EXTENTION1}|{SCRIPT_EXTENTION})
DELIMITADOR     [\t\r\f" "]
COMENTARIO      #([^#|^\n|^\r\n])*(#)?


%%
{ENTERO} {yylval.number = atoi(yytext); return number;}
{REAL} {yylval.number = atoi(yytext); return number;}
{NEGATIVO} {yylval.number = atoi(yytext); return number;}

"show" {return(show);}
"exec" {return(readfile);}
{PAUSE} {return(pausa);}
{MKDISK} {return(mkdisk);}
{RMDISK} {return(rmdisk);}
{FDISK} {return(fdisk);}
{MOUNT} {return(mount);}
{UNMOUNT} {return(unmount);}
{MKFS} {return(mkfs);}
{REP} {return(rep);}
{RM} {return(rm);}

{LOGIN} {return(login);}
{LOGOUT} {return(logout);}
{MKGRP} {return(mkgrp);}
{RMGRP} {return(rmgrp);}
{MKUSR} {return(mkusr);}
{RMUSR} {return(rmusr);}
{TOUCH} {return(touch);}

{CHMOD} {return(chmod);}
{MKFILE} {return(mkfile);}
{CAT} {return(cat);}
{REM} {return(rem);}
{EDIT} {return(edit);}
{REN} {return(ren);} 
{MKDIR} {return(mkdir);} 
{CP} {return(cp);} 
{MV} {return(mv);} 
{FIND} {return(findd);} 
{CHOWN} {return(chownn);} 
{CHGRP} {return(chgrp);} 

{SIZE} {return(size);}
{FIT} {return(fit);}
{UNITS} {return(units);}
{PATH} {return(path);}
{RUTA} {return(ruta);}
{ROOT} {return(root);}
{TYPE} {return(type);}
{NAME} {return(name);}
{DELET} {return(delet);}
{ADD} {return(add);}
{USER} {return(usr);}
{PASS} {return(pass);}
{ID} {return(id);}
{GROUP} {return(grp);}
{FORMAT} {return(fs);}
{RECURSIVE} {return(recursive);}
{UGO} {return(ugo);}
{CONT} {return(cont);}
{STDIN} {return(stdinn);} 
{FILEN} {return(filen);}
{PP} {return(pp);}
{DEST} {return(dest);}

{PATH1} {strcpy(yylval.text, yytext); return(e_path);}
{PATH2} {strcpy(yylval.text, yytext); return(e_path);}
{BF} {sscanf(yytext, "%s", yylval.entrance); return(e_fit);}
{FF} {sscanf(yytext, "%s", yylval.entrance); return(e_fit);}
{WF} {sscanf(yytext, "%s", yylval.entrance); return(e_fit);}
{KILOBY} {sscanf(yytext, "%s", yylval.entrance); return(e_units);}
{MEGBY} {sscanf(yytext, "%s", yylval.entrance); return(e_units);}
{BYTE} {sscanf(yytext, "%s", yylval.entrance); return(e_units);}
{E_E} {sscanf(yytext, "%s", yylval.entrance); return(e_type);}
{E_P} {sscanf(yytext, "%s", yylval.entrance); return(e_type);}
{E_L} {sscanf(yytext, "%s", yylval.entrance); return(e_type);}
{FAST} {sscanf(yytext, "%s", yylval.entrance); return(e_delet);}
{FULL} {sscanf(yytext, "%s", yylval.entrance); return(e_delet);}
{NAME1} {strcpy(yylval.text, yytext); return(e_name);}
{NAME2} {strcpy(yylval.text, yytext); return(e_name);}
{IDES} {strcpy(yylval.text, yytext); return(e_id);}
{PASSWORD} {strcpy(yylval.text, yytext); return(password);}

{DELIMITADOR} {}
{COMENTARIO} {strcpy(yylval.text, yytext); return(comentario);}// creo que este lo voy a tener que poner como delimitador porque el comentario no hace nada
{IGUAL} {return(igual);}
.               {printf("Caracter %s no admitido\n ", yytext);}

%%