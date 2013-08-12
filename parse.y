%{

#include <stdlib.h>
#include <stdio.h>

#include "command.h"
#include "make_cmd.h"

int EOF_Reached = 0;

static REDIRECTEE redir;
%}

%union {
	WORD_DESC *word;
	int number;
	WORD_LIST *word_list;
	COMMAND *command;
	REDIRECT *redirect;
	ELEMENT element;
}

%token <word> WORD
%token <number> NUMBER

%type <command> command simple_command simple_list pipeline pipeline_command
%type <redirect> redirection 
%type <element> simple_command_element

%start inputunit

%%

inputunit: simple_list simple_list_terminator {
		global_command = $1; 
		YYACCEPT;
	}
	| '\n' { 
		global_command = (COMMAND*)NULL;
		YYACCEPT;
	}
	| {
		EOF_Reached = 1;
		YYACCEPT;
	}
	;

simple_list_terminator: '\n' 
	;

simple_list: pipeline_command { $$ = $1; }
	;

command: simple_command { $$ = $1; }
	;

simple_command_element: WORD { $$.word = $1, $$.redirect = (REDIRECT*)NULL; }
	| redirection { $$.redirect = $1, $$.word =(WORD_DESC*)NULL; }
	;

simple_command: simple_command_element { $$ = make_simple_command($1, (COMMAND*)NULL); global_command = $$; }
	| simple_command simple_command_element { $$ = make_simple_command($2, $1); global_command = $$; }
	;

redirection:
	'<' WORD { 
		redir.filename = $2;
		$$ = make_redirection(0, r_input_direction, redir);			
	}
	| '>' WORD {
		redir.filename = $2;
		$$ = make_redirection(1, r_output_direction, redir);
	}
	;

pipeline_command:
	pipeline { $$ = $1; }
	;

pipeline:
	pipeline '|' pipeline {
		$$ = command_connect($1, $3, '|');
	}
	| command { $$ = $1; }
	;

%%
