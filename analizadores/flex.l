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

LOGIN       "login"
LOGOUT      "logout"
MKGRP       "mkgrp"
RMGRP       "rmgrp"
MKURS       "mkurs"
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

SIZE        "-size"
FIT         "-f"
UNITS       "-u"
PATH        "-path"
TYPE        "-type"
DELET      "-delete"
NAME        "-name"
ADD         "-add"
ID          "-id"
FORMAT      "-fs"

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
PATH1       {SLASH}{SEMIROUTE}+{NAME_FILE}
PATH2       \"{SLASH}({IDENTIFICADOR}|{ENTERO}|{SLASH}|" ")+{NAME_FILE}\"
NAME1       {IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO})*
NAME2       \"{IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO}|" ")*\"
IDES        {ENTERO}[a-z]{1,2}

IGUAL       "="
SLASH       "/"
SEMIROUTE   {IDENTIFICADOR}({IDENTIFICADOR}|{DIGITO})*{SLASH}

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

{LOGIN} {printf("login %s",yytext);}
{LOGOUT} {printf("logout %s",yytext);}
{MKGRP} {printf("mkgrp %s",yytext);}
{RMGRP} {printf("rmgrp %s",yytext);}
{MKURS} {printf("mkurs %s",yytext);}
{RMUSR} {printf("rmusr %s",yytext);}

{CHMOD} {printf("chmod %s",yytext);}
{MKFILE} {printf("mkfile %s",yytext);}
{CAT} {printf("cat %s",yytext);}
{REM} {printf("rem %s",yytext);}
{EDIT} {printf("edit %s",yytext);}
{REN} {printf("ren %s",yytext);} 
{MKDIR} {printf("mkdir %s",yytext);} 
{CP} {printf("cp %s",yytext);} 
{MV} {printf("mv %s",yytext);} 
{FIND} {printf("find %s",yytext);} 
{CHOWN} {printf("chown %s",yytext);} 
{CHGRP} {printf("chgrp %s",yytext);} 

{SIZE} {return(size);}
{FIT} {return(fit);}
{UNITS} {return(units);}
{PATH} {return(path);}
{TYPE} {return(type);}
{NAME} {return(name);}
{DELET} {return(delet);}
{ADD} {return(add);}
{ID} {return(id);}
{FORMAT} {}

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

{DELIMITADOR} {}
{COMENTARIO} {strcpy(yylval.text, yytext); return(comentario);}// creo que este lo voy a tener que poner como delimitador porque el comentario no hace nada
{IGUAL} {return(igual);}
.               {printf("Caracter %s no admitido\n ", yytext);}

%%