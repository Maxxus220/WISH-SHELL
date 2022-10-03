OBJS = wish.o builtIns.o utils.o
CFLAGS = -g
USER_COMMANDS = ./bin/test

wish : $(OBJS) $(USER_COMMANDS)
	gcc -o wish $(OBJS) $(FLAGS)

wish.o : wish.c builtIns.h utils.h

builtIns.o : builtIns.c builtIns.h

utils.o : utils.c utils.h

./bin/test : ./bin/test.c
	gcc -o ./bin/test ./bin/test.c

.PHONY : clean
clean :
	-rm $(OBJS) wish