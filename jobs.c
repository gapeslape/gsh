#include <sys/types.h>
#include <sys/wait.h>

pid_t last_made_pid;


int make_child()
{
	return (last_made_pid = fork());
}

int wait_for(pid_t pid)
{
	int status;

	if(waitpid(pid, &status, 0) != pid)
		return -1;

	return status;
}
