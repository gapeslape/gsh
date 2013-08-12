#if !defined (_EXECUTE_CMD_H_)
#define _EXECUTE_CMD_H_

#include "command.h"

#define NO_PIPE -1

extern int execute_command(COMMAND*);
extern int execute_command_internal(COMMAND*, int, int);
#endif
