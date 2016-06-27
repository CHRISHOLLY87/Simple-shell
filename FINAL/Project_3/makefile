all: ourshell

ourshell: ourshell.c
	gcc -g -Wall -Wl,--no-as-needed -ldl -lpthread ourshell.c -o ourshell

clean:
	rm *o ourshell
