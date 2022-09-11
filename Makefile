CC=gcc

CFLAGS=-g -Wall -D__USE_FIXED_PROTOTYPES__

OBJS=converter.o in_file.o ch_type.o token.o list.o stack.o syntax_tree.o

#dammy
.PHONY:all
all: converter

converter: $(OBJS)
	$(CC) $(CFLAGS) -o converter $(OBJS)

converter.o: converter.c in_file.h ch_type.h token.h

in_file.o: in_file.c in_file.h
	$(CC) $(CFLAGS) -c in_file.c

ch_type.o: ch_type.c ch_type.h
	$(CC) $(CFLAGS) -c ch_type.c

token.o: token.c token.h ch_type.h in_file.h
	$(CC) $(CFLAGS) -c token.c

list.o: list.c list.h token.h in_file.h

stack.o: stack.c stack.h token.h list.h

syntax_tree.o: syntax_tree.h stack.h token.h

#dammy
.PHONY: clean
clean:
	rm -f converter $(OBJS)
