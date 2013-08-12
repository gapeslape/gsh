#include "command.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/fcntl.h>

WORD_LIST* make_word_list(WORD_DESC *word, WORD_LIST* list)
{
	WORD_LIST* elem = (WORD_LIST*)malloc(sizeof(WORD_LIST));
	elem->word = word;
	elem->next = (WORD_LIST*)NULL;

	if(list == (WORD_LIST*)NULL)
		return elem;

	WORD_LIST* p = list;

	while(p->next) p = p->next;
	
	p->next = elem;

	return list;
}

COMMAND* make_command(enum command_type type, SIMPLE_COM* pointer)
{
	COMMAND* temp = (COMMAND*)malloc(sizeof(COMMAND));

	temp->type = type;
	temp->value.simple = pointer;
	temp->flags = 0;

	return temp;
}


COMMAND* make_simple_command(ELEMENT element, COMMAND* command)
{
	if(command == (COMMAND*)NULL)
	{
		SIMPLE_COM* simple = (SIMPLE_COM*)malloc(sizeof(SIMPLE_COM));
		simple->redirects = 0;
		simple->words = 0;
		simple->flags = 0;
		command = make_command(cm_simple, simple);
	}

	SIMPLE_COM* simple = command->value.simple;

	if(element.word)
	{
		WORD_LIST* list = (WORD_LIST*)malloc(sizeof(WORD_LIST));

		list->word = element.word;
		list->next = NULL;

		if(simple->words == NULL)
			simple->words = list;
		else
		{
			WORD_LIST* p = simple->words;
			

			while(p->next) p = p->next;

			p->next = list;
		}
	}

	if(element.redirect)
	{
		if(simple->redirects == (REDIRECT*)NULL)
			simple->redirects = element.redirect;
		else
		{
			REDIRECT* redirection = simple->redirects;

			while(redirection->next) redirection = redirection->next; 

			redirection->next = element.redirect;
		}
	}
		
	return command;	
}

REDIRECT* make_redirection(int source, enum r_instruction instruction, REDIRECTEE redirectee)
{
	REDIRECT* temp = (REDIRECT*)malloc(sizeof(REDIRECT));

	temp->redirector = source;
	temp->instruction = instruction;
	temp->redirectee = redirectee;
	temp->next = (REDIRECT*)NULL;

	switch(instruction)
	{
	case r_input_direction:
		temp->flags = O_RDONLY;
		break;
	case r_output_direction:
		temp->flags = O_WRONLY | O_CREAT;
		break;
	}

	return temp;
}

COMMAND* command_connect(COMMAND* com_1, COMMAND* com_2, int connector)
{
	CONNECTION* connection = (CONNECTION*)malloc(sizeof(CONNECTION));

	connection->first = com_1;
	connection->second = com_2;
	connection->connector = connector;

	return make_command(cm_connection, (SIMPLE_COM*)connection); 
} 
