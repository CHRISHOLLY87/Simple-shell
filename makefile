CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I.
LIBS = -lpthread
SRCS = ourshell.c csapp.c p3extra.c
OBJS = ourshell.o csapp.o p3extra.o
EXE = upsh
LDFLAGS = -ldl -Wl,--no-as-needed


all:	$(EXE)

$(EXE):	$(OBJS) 
	$(CC) -o $(EXE) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) ​*.o *​~ $(EXE)
