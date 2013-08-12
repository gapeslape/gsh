all: gsh

SRCS=shell.c make_cmd.c eval.c \
     execute_cmd.c jobs.c redir.c \
     externs.c parse.tab.c lex.yy.c 

parse.tab.c: parse.y
	@echo "building grammar parser (bison)..."
	@bison -d parse.y
lex.yy.c: parse.l
	@echo "building lexiographical parser (flex)..."
	@flex parse.l
gsh: $(SRCS)
	@echo "compiling gsh (gcc)..."
	@gcc $(SRCS) -lfl -g -o gsh
	@echo "success"
clean:
	@echo "cleaning..."
	@rm -rf 2>/dev/null parse.tab.c parse.tab.h lex.yy.c *.o gsh

install: 
	@echo "installing..."
	@sudo cp gsh /usr/bin
