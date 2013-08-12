#include "redir.h"

#include <stdlib.h>
#include <stdio.h>

int do_redirections(REDIRECT* redirect)
{
	while(redirect)
	{

		switch(redirect->instruction)
		{
		case r_input_direction:
		case r_output_direction:
		{
			int fd = open(redirect->redirectee.filename->word, redirect->flags, 0644);
			if( fd < 0 )
			{
				fprintf(stderr, "open failed for %s in do_redirections()\n", redirect->redirectee.filename->word);
				return -1;
			}
	
			if( dup2(fd, redirect->redirector) < 0 )
			{
				fprintf(stderr, "dup2 failed in do_redirections()\n");
				return -1;
			}
		} break;
		default:
		{
			fprintf(stderr, "programming error: wrong redirect instruction type");
			exit(-1);
		}
		}

		redirect = redirect->next;
	}

	return 0;
}
