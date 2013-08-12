#if !defined(_COMMAND_H_)
#define _COMMAND_H_

typedef struct word_desc {
	char *word;
	int flags;
} WORD_DESC;

typedef struct word_list {
	struct word_list* next;
	WORD_DESC* word;
} WORD_LIST;

enum r_instruction {
	r_output_direction, r_input_direction
};

typedef union {
	int dest;
	WORD_DESC *filename;
} REDIRECTEE;

typedef struct redirect {
	struct redirect* next;
	int redirector;
	int flags;
	enum r_instruction instruction;
	REDIRECTEE redirectee;
} REDIRECT;

typedef struct element {
	WORD_DESC* word;
	REDIRECT* redirect;
} ELEMENT;

enum command_type {
	cm_simple,
	cm_connection
};

typedef struct command {
	enum command_type type;
	int flags;
	int line;

	union {
		struct simple_com *simple;
		struct connection *connection;
	} value;
} COMMAND;

typedef struct simple_com {
	int flags;
	int line;
	WORD_LIST *words;
	REDIRECT *redirects;
} SIMPLE_COM;

typedef struct connection {
	COMMAND* first;
	COMMAND* second;
	int connector;
} CONNECTION;

extern COMMAND* global_command;
#endif
