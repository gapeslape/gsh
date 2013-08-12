%{
#include "command.h"
#include "parse.tab.h"

%}

%%
"\n"|"|"|">"|"<" { return yytext[0]; }
[ \t] /* eat up white characters */
#.* /* eat up comments */
\"[^\"\n]*\" {
	yylval.word = (WORD_DESC*)malloc(sizeof(WORD_DESC));

	yylval.word->word = strdup(yytext + 1);
	yylval.word->word[strlen(yytext) - 2] = '\0';

	return WORD;
}

[^ \t\n]* { 
	yylval.word = (WORD_DESC*)malloc(sizeof(WORD_DESC));
	yylval.word->word = strdup(yytext); return WORD; }
