/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    show = 258,
    pausa = 259,
    comentario = 260,
    mkdisk = 261,
    rmdisk = 262,
    fdisk = 263,
    mount = 264,
    unmount = 265,
    mkfs = 266,
    rep = 267,
    ruta = 268,
    root = 269,
    chmod = 270,
    touch = 271,
    cat = 272,
    rm = 273,
    edit = 274,
    ren = 275,
    rem = 276,
    findd = 277,
    mkdir = 278,
    cp = 279,
    mv = 280,
    mkfile = 281,
    chownn = 282,
    chgrp = 283,
    size = 284,
    fit = 285,
    units = 286,
    igual = 287,
    grp = 288,
    ugo = 289,
    recursive = 290,
    cont = 291,
    stdinn = 292,
    filen = 293,
    pp = 294,
    dest = 295,
    path = 296,
    type = 297,
    name = 298,
    add = 299,
    id = 300,
    delet = 301,
    format = 302,
    readfile = 303,
    fs = 304,
    number = 305,
    e_path = 306,
    password = 307,
    e_fit = 308,
    e_units = 309,
    e_type = 310,
    e_delet = 311,
    e_name = 312,
    e_id = 313,
    login = 314,
    logout = 315,
    pass = 316,
    usr = 317,
    mkgrp = 318,
    rmgrp = 319,
    mkusr = 320,
    rmusr = 321
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 110 "bison.y"

    int number;
    char text[200];
    char entrance[12];
    int other;

#line 131 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
