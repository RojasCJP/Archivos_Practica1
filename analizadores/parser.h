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
    chmod = 267,
    touch = 268,
    cat = 269,
    rm = 270,
    edit = 271,
    ren = 272,
    mkdir = 273,
    cp = 274,
    mv = 275,
    size = 276,
    fit = 277,
    units = 278,
    igual = 279,
    grp = 280,
    ugo = 281,
    recursive = 282,
    cont = 283,
    stdin = 284,
    filen = 285,
    pp = 286,
    dest = 287,
    path = 288,
    type = 289,
    name = 290,
    add = 291,
    id = 292,
    delet = 293,
    format = 294,
    readfile = 295,
    fs = 296,
    number = 297,
    e_path = 298,
    e_name = 299,
    password = 300,
    e_fit = 301,
    e_units = 302,
    e_type = 303,
    e_delet = 304,
    e_id = 305,
    login = 306,
    logout = 307,
    pass = 308,
    usr = 309,
    mkgrp = 310,
    rmgrp = 311,
    mkusr = 312,
    rmusr = 313
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 81 "bison.y"

    int number;
    char text[200];
    char entrance[12];
    int other;

#line 123 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
