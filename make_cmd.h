#if !defined(_MAKE_CMD_H_)
#define _MAKE_CMD_H_

extern WORD_LIST *make_word_list(WORD_DESC*, WORD_LIST*);
extern COMMAND* make_simple_command(ELEMENT element, COMMAND* command);
extern REDIRECT* make_redirection(int source, enum r_instruction instruction, REDIRECTEE redirectee);
extern COMMAND* command_connect(COMMAND*, COMMAND*, int);

#endif
