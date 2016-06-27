CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I.
LIBS = -lpthread
SRCS = upsh.c csapp.c p3extra.c -ldl
OBJS = upsh.o csapp.o p3extra.o -ldl
EXE = upsh

all:	$(EXE)

$(EXE):	$(OBJS)
	$(CC) -o $(EXE) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(EXE)
