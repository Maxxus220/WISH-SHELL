OBJS = wish.o builtIns.o utils.o
CFLAGS = -g
USER_COMMANDS = test

wish : $(OBJS) $(USER_COMMANDS)
	gcc -o wish $(OBJS) $(FLAGS)

wish.o : wish.c builtIns.h utils.h

builtIns.o : builtIns.c builtIns.h

utils.o : utils.c utils.h

test : test.c
	gcc -o test test.c

.PHONY : clean
clean :
	-rm $(OBJS) wish