
#include "command.h"
#include "shell.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <string.h>

extern FILE *yyin;

int reader_loop()
{
	while( EOF_Reached == 0 )
	{
		maybe_print_prompt();

		if(read_command() == 0)
		{
			COMMAND* current_command = global_command;
			global_command = (COMMAND*)NULL;

			execute_command(current_command);
			
			// TODO: write dispose_command
			//dispose_command(current_command);
			current_command = (COMMAND*)NULL;
		}
	}
}

int read_command()
{
	return yyparse();
}

int maybe_print_prompt()
{
	char hostname[256];
	char dir[1024];

	if(!interactive_shell)
		return -1;

	gethostname(hostname, sizeof(hostname));
	getcwd(dir, sizeof(dir));

	struct passwd *passwd = getpwuid(getuid());

	if(passwd == 0) return -1;

	if(strncmp(dir, passwd->pw_dir, strlen(passwd->pw_dir)) == 0)
	{
		char tmp[1024];
		sprintf(tmp, "~%s", dir + strlen(passwd->pw_dir));
		strcpy(dir, tmp);
	}

	printf("%s@%s:%s$ ", passwd->pw_name, hostname, dir);

	return 0;	
}
