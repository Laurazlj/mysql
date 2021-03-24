%{
#include "common.h"
#include "parser.tab.hpp"

char* tokenctx;

extern "C" int yywrap(void)
{
 return 1;
}

%}

%option nounput
%option noinput
digit [0-9]
digits {digit}+


%%
" "|"\t" {}
"\n"	   {}
"CREATE" {return CREATE;}
"DATABASE"  {return DATABASE;}
"USE"  {return USE;}
"SHOW"  {return SHOW;}
"DATABASES" {return DATABASES;}
"DROP"  {return DROP;}
"TABLE"  {return TABLE;}
"TABLES" {return TABLES;}
"INT"  {return INT;}
"CHAR"  {return CHAR;}
"INSERT" {return INSERT;}
"INTO"  {return INTO;}
"VALUES" {return VALUES;}
"SELECT" {return SELECT;}
"FROM"  {return FROM;}
"WHERE"  {return WHERE;}
"AND"  {return AND;}
"OR"  {return OR;}
"NOT"  {return NOT;}
"TRUE"  {return TRUE;}
"FALSE"  {return FALSE;}
"DELETE" {return DELETE;}
"UPDATE" {return UPDATE;}
"SET"  {return SET;}
"EXIT"  {return EXIT;}
"QUIT"  {return QUIT;}
"PRIMARY"  {return PRIMARY;}
"KEY"    {return KEY;}
"("   {return '(';}
")"   {return ')';}
","   {return ',';}
";"   {return ';';}
"+"   {return '+';}
"-"   {return '-';}
"*"   {return '*';}
"/"   {return '/';}
">"   {return '>';}
"<"   {return '<';}
"="   {return '=';}
"!"   {return '!';} 
"\'"  {return '\'';}
{digits} {
   tokenctx = (char*)malloc(strlen(yytext)+1);
   strcpy(tokenctx,yytext);
   return NUMBER;
}
   
[a-zA-Z][a-zA-Z0-9_]* {
   tokenctx=(char*)malloc(strlen(yytext)+1);
   strcpy(tokenctx,yytext);
   return ID;
}
[a-zA-Z0-9_]* {
   tokenctx=(char*)malloc(strlen(yytext)+1);
   strcpy(tokenctx,yytext);
   return STRING;
}
%%