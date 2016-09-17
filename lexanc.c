/* lex1.c         14 Feb 01; 31 May 12       */

/* This file contains code stubs for the lexical analyzer.
   Rename this file to be lexanc.c and fill in the stubs.    */

/* Copyright (c) 2001 Gordon S. Novak Jr. and
   The University of Texas at Austin. */

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexan.h"

/* This file will work as given with an input file consisting only
   of integers separated by blanks:
   make lex1
   lex1
   12345 123    345  357
*/

const char* operators[] = { "+", "-", "*", "/", ":=", "=", "<>", "<", "<=", ">=", ">", "^", "and", "or", "not", "div", "mod", "in", "if", "goto", "progn", "label", "funcall", "aref", "program", "float" };
const char* delimiters[] = { ",", ";", ":", "(", ")", "[", "]", ".." };
const char* numbertypes[] = { "INTEGER", "REAL", "STRINGTYPE", "BOOLETYPE", "POINTER" };



/* Skip blanks and whitespace.  Expand this function to skip comments too. */
void skipblanks ()
{
    int c;
    int d;
    while ((c = peekchar()) != EOF && (c == ' ' || c == '\n' || c == '\t'
            || c == '{' || (c == '(' && (d = peek2char()) == '*')))
    {    
        //skip {}
        if(c == '{')
        {
          getchar();
          while( ((c = peekchar()) != EOF) && c != '}' )
            getchar();
        }

        //skip (**)
        else if(c == '(')
        {
            //skip first two chars
            getchar();
            getchar();
            c = d;
            d = peek2char();

            while (d != EOF && (c != '*' || d != ')'))
            {
                getchar();
                c = d;
                d = peek2char();
            }
            getchar();
            c = d;
        }
        if(c != EOF)
        {
            getchar();
        }
    }
}

/* Get identifiers and reserved words */
TOKEN identifier (TOKEN tok)
{
    int c = 0, d = 0;
    int count = 0;
    char str[16]; //max string
    
}

//strings
TOKEN getstring (TOKEN tok)
{
    int c = peek2char();
    getchar();
    int d = peek2char();

    int count = 0;
    char str[16];
    printf("Value of c is: %c", c);
    printf("\nValue of d is: %c\n", d);

    while(c != EOF)
    {
        if(c == '\'')
            if(d == '\'')
                getchar();
            else
                goto exit;
        if(count < 15)
        {
            str[count] = getchar();
            count += 1;
        }
        else
            getchar();
        c = peekchar();
        d = peek2char();
    }
    exit:
    getchar();
    //terminate string
    str[count] = '\0';
    printf("String --> %s, count = %i\n", str, count);
    tok->tokentype = STRINGTOK;
    strncpy(tok->stringval, str, 16);
}
//delimiters and operators
TOKEN special (TOKEN tok)
{
    int c = peekchar();
    int d = peek2char();
    printf("c = %c\n", c);
    printf("d = %c\n", d);
    int op = 0, dm = 0;
    int i = 0, j = 0;
    for (; i < (sizeof operators / sizeof *operators); i++)
    {
      printf("i = %i, size = %i\n", i, sizeof operators / sizeof *operators);
      char p = operators[i][0];
      if(p == c)
      {
        if(strlen(operators[i]) > 1)
        {
          char q = operators[i][1];
          if(c == p && d == q)
          {
            op = 1;
            break;
          }
        }
        else
        {
          if(c == p )
          {
            op = 1;
            break;
          }
        }
      }
    }
    printf("midway\n");
    for (; op==0 && j < ((sizeof delimiters / sizeof *delimiters)); j++)
    {
      printf("j = %i\n", j);
      char p = delimiters[j][0];
      if(p == c)
      {
        if(strlen(delimiters[j]) > 1)
        {
          char q = delimiters[j][1];
          if(c == p && d == q)
          {
            dm = 1;
            break;
          }
        }
        else
        {
          if(c == p )
          {
            dm = 1;
            break;
          }
        }
      }
    }
    printf("op = %c, i = %i\n", op, ++i);
    printf("dm = %c, j = %i\n", dm, ++j);

    if(op > dm)
    {
      tok->tokentype = OPERATOR;
      tok->whichval = i;
      printf("-----OPERATOR: \'%c%c\' is\n", c, d);
    }
    else if(op < dm)
    {
      tok->tokentype = DELIMITER;
      tok->whichval = j;
      printf("-----DELIMITER: \'%c%c\' is\n", c, d);
    }
    else
    {
      printf("ERROR: \'%c%c\' is unrecognized\n", c, d);
    }
    getchar();
    return tok;
}

/* Get and convert unsigned numbers of all types. */
TOKEN number (TOKEN tok)
{ 
    long num;
    int  c, charval;
    num = 0;
    while ( (c = peekchar()) != EOF && CHARCLASS[c] == NUMERIC)
    {   
        c = getchar();
        charval = (c - '0');
        num = num * 10 + charval;
    }
    tok->tokentype = NUMBERTOK;
    tok->datatype = INTEGER;
    tok->intval = num;
    return (tok);
}
