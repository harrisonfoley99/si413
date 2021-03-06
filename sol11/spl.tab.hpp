/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_SPL_TAB_HPP_INCLUDED
# define YY_YY_SPL_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 9 "spl.ypp" /* yacc.c:1909  */


#include <cstdlib>
#include <iostream>
using namespace std;

#include "ast.hpp"
#include "readlineistream.hpp"

int yylex();
int yylex_destroy();


#line 58 "spl.tab.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BOP = 258,
    NOTTOK = 259,
    COMP = 260,
    OPA = 261,
    OPM = 262,
    POSNEG = 263,
    FUNARG = 264,
    LC = 265,
    RC = 266,
    LP = 267,
    RP = 268,
    LAMBDA = 269,
    IF = 270,
    IFELSE = 271,
    WHILE = 272,
    READ = 273,
    WRITE = 274,
    NEW = 275,
    ASN = 276,
    STOP = 277,
    ID = 278,
    NUM = 279,
    BOOL = 280,
    DEBUG = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 52 "spl.ypp" /* yacc.c:1909  */

  Block* block;
  Stmt* stmt;
  Exp* exp;
  Id* id;
  Oper op;

#line 105 "spl.tab.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SPL_TAB_HPP_INCLUDED  */
