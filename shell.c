#include "command.h"
#include "eval.h"

#include <stdio.h>
#include <unistd.h>

void process_simple_command(COMMAND* command);

extern FILE* yyin;

COMMAND* global_command;
int interactive_shell;


int main(int argc, const char* argv[])
{
	--argc, ++argv;

	if( argc > 0 && argv[0] )
	{
		fprintf(stderr, "gsh (version 0.0): executing script %s\n", argv[0]);
		yyin = fopen(argv[0], "r");
	}	
	else
		yyin = stdin;

	interactive_shell = 0;
	if(isatty(fileno(yyin)))
		interactive_shell = 1;

	reader_loop();
}

void yyerror(const char* msg)
{
	fprintf(stderr, "error parsing: %s\n", msg);
}
