OBJS = wish.o builtIns.o utils.o
CFLAGS = -g
USER_COMMANDS = ./bin/test2

wish : $(OBJS) $(USER_COMMANDS)
	gcc -o wish $(OBJS) $(FLAGS)

wish.o : wish.c builtIns.h utils.h

builtIns.o : builtIns.c builtIns.h

utils.o : utils.c utils.h

./bin/test : ./bin/test2.c
	gcc -o ./bin/test2 ./bin/test2.c

.PHONY : clean
clean :
	-rm $(OBJS) wish