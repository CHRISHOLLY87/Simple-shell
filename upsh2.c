/* $begin shellmain */
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "csapp.h"
#include "p3extra.c"
#include "csapp.h"
#include "csapp.c"
#define MAXARGS   128

/* function prototypes */
void eval(char *cmdline);
int p3parseline(char *buf, char **argv); /* new parseline function for cs485 project 3 */
int builtin_command(char **argv);
void initialize_built_in();
int execute_built_in(char **argv);

struct NewBuiltIn {
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};
struct NewBuiltIn *pluggin_method;
char *builtin_files[8] =  {
	"./comment.so", "./cd.so", "./bgjobs.so", "./fg.so", "./culater.so"
};
char *builtin_functions[64] =  {
	"comment", "cd", "bgjobs", "fg", "culater"
};
char *prompt = "upsh";
typedef void (*func_ptr)(char **);
func_ptr function;
void *testfunction(char **);
void *pointers_to_functions[64] = {testfunction};
char *background_jobs[64];
int ptr_to_func_index = 7;
int bg_index = 0;


int main(int argc, char *argv[])
{
    char cmdline[MAXLINE]; /* Command line */
		initialize_built_in(); //initialize pointers to functions for the already built in commands
    while (1) {
			/* Read */
		printf("%s> ", prompt);
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
	    exit(0);

		/* Evaluate */
		eval(cmdline);
    }
}

void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */

    strcpy(buf, cmdline);
    bg = p3parseline(buf, argv);

	if (argv[0] == NULL)
	return;   /* Ignore empty lines */

	//analyzer function call?

	if (!strcmp(argv[0], "quit")){
		exit(0);
	}

  if (builtin_command(argv)) {
		background_jobs[bg_index] = argv[0];
		bg_index++;
		if ((pid = fork()) == 0) {   /* Child runs user job */
	    if (execve(argv[0], argv, environ) < 0) {
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
	    }
		}
	}
	else {
		//execute built in command
		if (execute_built_in(argv) < 0){
			printf("blah error in execute_built_in\n");
		}
	}

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0)
				printf("blah error");
				//unix_error("waitfg: waitpid error");
	}
	else
	    printf("%d %s", pid, cmdline);
    return;
}

	/*
		this function should check if the command is a built in command
		if it is not then it should return false
		if it is a built in command then it will return teuw
	*/
int builtin_command(char **argv)
{
		int i;
		for (i = 0; i <64; i++){
			if (builtin_functions[i] == argv[0]){
				return 1;	//the command is already built in
			}
		}

		return 0;  //the command is not builtin therefore it must be a unix command
}

/*
	this function sees an array of pointers to functions as a global variable
	it must scan through the built in functions array and find the correct index
		this index will be equal to the index of the pointer to the function in the ptr array
*/
int execute_built_in(char **argv){

	int i;
	for (i = 0; i < sizeof(builtin_functions); i++){
		if (i == 0){
			function = pointers_to_functions[1];
			char *bleh[] = {"bleh"};
			function(bleh);
		}
		if (!strcmp(argv[0], builtin_functions[i])){
			//the index has been found
			function = pointers_to_functions[i];
			function(argv); //execute function
			return 0;
		}
	}
	return -1;
}

/*
	this function initilizes all the pointers to functions
	for the default built in commands
	it appends each pointer to a function to the array pointers_to_functions
*/
void initialize_built_in(){
	void *handle;
	char *error;
	int i;
	for (i = 1; i < 5; i++){
		handle = dlopen(builtin_files[i], RTLD_LOCAL | RTLD_LAZY);
		if (!handle) {
 			fprintf(stderr, "%s\n", dlerror());
 			exit(1);
 		}
		pointers_to_functions[i] = dlsym(handle, builtin_functions[i]);
		if ((error = dlerror()) != NULL) {
 			fprintf(stderr, "%c\n", *error);
 			exit(1);
 		}

		if (dlclose(handle) < 0) {
		 	fprintf(stderr, "%c\n", *dlerror());
		 	exit(1);
		 }
	}
}

void *testfunction(char **argv){
	printf("WOOHOO!\n");
	return 0;
}
