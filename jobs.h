#if !defined (_JOBS_H_)
#define _JOBS_H_

#include <sys/types.h>

extern pid_t last_made_pid;

extern int make_child();
extern int wait_for(pid_t);

#endif
