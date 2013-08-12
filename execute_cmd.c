#include "execute_cmd.h"
#include "jobs.h"
#include "shell.h"
#include "externs.h"

#include <stdio.h>
#include <stdlib.h>

int execute_command(COMMAND* command)
{
	return execute_command_internal(command, NO_PIPE, NO_PIPE);
}

int execute_command_internal(COMMAND* command,
	int pipe_in,
	int pipe_out)
{
	int exec_result;

	if(command == (COMMAND*)NULL) return (EXECUTION_SUCCESS);

	switch(command->type)
	{
	case cm_simple:
	{
		exec_result = execute_simple_command(command->value.simple, pipe_in, pipe_out);

		// execition failed, break out here
		if(exec_result) break;		

		/* we are the parent, simply wait for last executed child */
		exec_result = wait_for(last_made_pid);
	} break;	
	case cm_connection:
	{
		exec_result = execute_connection(command, pipe_in, pipe_out);
	} break;
	default:
	{
		fprintf(stderr, "programming error: unsupported command type\n");
		exit(-1);
	}
	}

	return exec_result;
}

int execute_simple_command(SIMPLE_COM* simple, int pipe_in, int pipe_out)
{
	return execute_disk_command(simple->words, simple->redirects, pipe_in, pipe_out);
}

int execute_disk_command(WORD_LIST* words, REDIRECT* redirects, int pipe_in, int pipe_out)
{
	/* TODO: move this to ./builtins/common.c */
	if(strcmp(words->word->word, "cd") == 0 && words->next->word->word)
	{
		if(words->next)
			chdir(words->next->word->word);
		return EXECUTION_SUCCESS;
	}

	if(strcmp(words->word->word, "exit") == 0)
		exit(EXECUTION_SUCCESS);
	/* TODO: end */

	pid_t pid = make_child();

	if(pid == 0)
	{
		do_piping(pipe_in, pipe_out);

		if(redirects && do_redirections(redirects) != 0)
		{	
			fprintf(stderr, "redirections failed\n");
			return (EXECUTION_FAILURE);
		}

		char* command = words->word->word;
		char** args = strvec_from_word_list(words);

		exit(shell_execve(command, args, (char**)NULL));
	}

	return 0; 
}

int execute_connection(COMMAND* command, int pipe_in, int pipe_out)
{
	switch(command->value.connection->connector)
	{
	case '|' :
		return execute_pipeline(command, pipe_in, pipe_out);
	default:
		fprintf(stderr, "programming error: unsupported connector\n");
		break;
	}

	return (EXECUTION_FAILURE);
}

int execute_pipeline(COMMAND* command, int pipe_in, int pipe_out)
{
	int filedes[2], prev, exec_result;

	COMMAND* cmd = command;
	prev = pipe_in;

	while(cmd && cmd->type == cm_connection && cmd->value.connection->connector == '|')
	{
		if(pipe(filedes) < 0)
		{
			fprintf(stderr, "error: pipe() failed\n");
			exit(EXECUTION_FAILURE);
		}

		execute_command_internal(cmd->value.connection->first, prev, filedes[1]);

		if(prev >= 0)
			close(prev);

		prev = filedes[0];
		close(filedes[1]);

		cmd = cmd->value.connection->second;
	}
	
	exec_result = execute_command_internal(cmd, prev, pipe_out);

	if(prev >= 0)
		close(prev);

	return (exec_result);
}

int shell_execve(char* command, char** args, char** export_env)
{
	if(*command == '/')
		execv(command, args);
	else
		execvp(command, args);

	fprintf(stderr, "exec failed for command: %s\n", command);

	return EXECUTION_FAILURE;
}

int do_piping(int pipe_in, int pipe_out)
{
	if(pipe_in != NO_PIPE)
		dup2(pipe_in, 0);
	if(pipe_out != NO_PIPE)
		dup2(pipe_out, 1);

	return 0;
}
