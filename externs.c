#include "externs.h"

#include <stdlib.h>

char** strvec_from_word_list(WORD_LIST* list)
{
	if(list == (WORD_LIST*)NULL) return (char**)NULL;

	// first count list lenght
	int count = 1; // we have at least one
	WORD_LIST* p = list;

	while(p = p->next) ++count;

	char** args = (char**)malloc(sizeof(char*)*(count + 1));

	p = list;
	int i = 0;
	while(p) { args[i++] = p->word->word; p = p->next; }

	args[i] = (char*)NULL;

	return args;
}
