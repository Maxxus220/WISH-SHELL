OBJS = wish.o builtIns.o utils.o

wish : $(OBJS)
	gcc -o wish $(OBJS)

wish.o : wish.c builtIns.h utils.h

builtIns.o : builtIns.c builtIns.h

utils.o : utils.c utils.h

.PHONY : clean
clean :
	-rm $(OBJS) wish