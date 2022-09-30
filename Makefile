OBJS = wish.o builtIns.o utils.o
CFLAGS = -g

wish : $(OBJS)
	gcc -o wish $(OBJS) $(FLAGS)

wish.o : wish.c builtIns.h utils.h

builtIns.o : builtIns.c builtIns.h

utils.o : utils.c utils.h

.PHONY : clean
clean :
	-rm $(OBJS) wish