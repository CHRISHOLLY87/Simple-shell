#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "csapp.h"
#include "p3extra.c"
#include "csapp.h"
#include "csapp.c"
#define MAXARGS   128
#define BUFLEN 64

/*function prototypes*/
void eval(char *cmdline);
int builtin_command(char **argv);
void initialize_builtin();
void execute_user_defined(char **argv);
void comment(char **argv);
void setprompt(char **argv);
void cd(char **argv);
void bgjobs();
void fg(int job_num);
void loadpluggin(char **argv);
void culater();
void file_redirection(char **);

//the following struct is to be used in user defined pluggins
struct NewBuiltIn {
	char CommandName[BUFLEN];
	char FunctionName[BUFLEN];
	char AnalyzerName[BUFLEN];
};
struct NewBuiltIn pluggin_method;

char *prompt = "upsh"; //global variable to be updated by setprompt()
char *background_jobs[BUFLEN]; //char array to hold background processes names
pid_t pids[BUFLEN]; //array to hold background processes pids
int job_numbers[BUFLEN]; //array to hold background job numbers -- used in bgjobs
int job_index = 1; //index for background jobs
char *commands[BUFLEN]; //array to hold command strings
char *functions[BUFLEN]; //array to hold function strings
char *analyzers[BUFLEN]; //array to hold anaylzer strings
int pluggin_index = 0; //index for user defined pluggins
char *builtin_commands[BUFLEN]; //array to hold commands that are built in OR have been dynamically loaded
int (*function[BUFLEN])(char **); //pointer to function array
int builtin_index = 7; //index for builtin_commands

int main(int argc, char *argv[])
{
    char cmdline[MAXLINE]; /* Command line */
		initialize_builtin(); //initialize builtin commands
    while (1) { //loop until user enters quit or ctrl-z
			/* Read */
		printf("%s> ", prompt);
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
	    exit(0);
		/* Evaluate */
		eval(cmdline);
    }
}

/*
	this function evaluates the command
		pseudocode
			parses the input
			determines if the command is already built in (default or linked)
			if it is not
				then check if the user defined it as a background command
				then call fork and execve
			if it is built in
				then check if it is a default
					call function is it is
					execute pointer to function if it is not
*/
void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds dified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */

    strcpy(buf, cmdline); //copy input into buf

    bg = p3parseline(buf, argv); /* call new parseline function for cs485 project 3 */
    if (argv[0] == NULL)
	     return;   /* Ignore empty lines */

		if (!strcmp(argv[0], "quit")) /* quit command */
			exit(0);

    if (builtin_command(argv) == 1) {
			//this command has not yet been built in
			if (bg){
						//insert the background process in each array
						background_jobs[job_index] = argv[0];
						job_numbers[job_index] = job_index;
						pids[job_index] = pid;
						job_index++;
			}
				//command is not built in
        if ((pid = fork()) == 0) {   /* Child runs user job */
          //check if the process is to be run in the background
          if (execve(argv[0], argv, environ) < 0) {
            printf("%s: Command not found.\n", argv[0]);
            exit(0);
        }
      }

			/* Parent waits for foreground job to terminate */
  		if (!bg) {
	    	int status;
				int err = waitpid(pid, &status, 0);
				if (err < 0) //check for error in waitpid
					unix_error("waitfg: waitpid error");
	    }
			else
	    	printf("%d %s", pid, cmdline);
  		}
  	else{
    	//this is a builtin command
			if (strcmp(argv[0], "%") == 0){
				comment(argv);
			}
			else if (strcmp(argv[0], "setprompt") ==0){
				setprompt(argv);
			}
			else if (strcmp(argv[0], "cd") == 0){
				cd(argv);
			}
			else if (strcmp(argv[0], "bgjobs") == 0){
				bgjobs();
			}
			else if (strcmp(argv[0], "fg") == 0){
				int job_num = atoi(argv[1]); //convert char array to integer
				//find the pid to that job number
				int i;
				for (i = 0; i < BUFLEN; i++){
					if (job_numbers[i] == job_num){
						pid_t pid_for_fg;
						pid_for_fg  = pids[i];
					}
				}
				fg(pid);
			}
			else if (strcmp(argv[0], "loadpluggin") == 0){
				loadpluggin(argv);
			}
			else if (strcmp(argv[0], "culater") == 0){
				culater();
			}
			else{
				int j;
				for (j = 0; j < builtin_index; j++){
					if (!strcmp(argv[0], builtin_commands[j])){
						execute_user_defined(argv);
				}
			}
		}
  }
  return;
}

/*
	this command checks if the command is already built in
		that is, if it is a default command or already linked
		user defined command
*/
int builtin_command(char **argv)
{
		int i;
		for (i = 0; i < builtin_index; i++){
			if (!strcmp(argv[0], builtin_commands[i]))
				return 0; //this command is already built in
		}
    return 1; //not a builtin command
}

/*
	this function is a sort of hack job to initialize builtin_commands
		given more time we would've determined a cleaner method
	the reason for this function is so that builtin_commands has leftover
		space to input user defined commands
*/
void initialize_builtin(){
	int i;
	for (i = 0; i < 7; i++){
		if (i == 0)
			builtin_commands[i] = "%";
		else if (i == 1)
			builtin_commands[i] = "setprompt";
		else if (i == 2)
			builtin_commands[i] = "cd";
		else if (i == 3)
			builtin_commands[i] = "bgjobs";
		else if (i == 4)
			builtin_commands[i] = "fg";
		else if (i == 5)
			builtin_commands[i] = "loadpluggin";
		else if (i == 6)
			builtin_commands[i] = "culater";
	}
}

/*
	this function executes the command
*/
void execute_user_defined(char **argv){
	file_redirection(argv); //check for file redirections
  //find the pointer to the function by finding the index of the command in commands
  int i;
  for (i = 0; i < pluggin_index; i++){
    if (!strcmp(argv[0], commands[i])){
			(function[i])(argv);
    }
  }
}

/*
	this command allows the user to input a comment
		given more time, we would've implemented a way to
		redirect the comment into a file
*/
void comment(char **argv){
  printf("You entered a comment... we are in the midst of processing it...\n");
}

/*
	this commmand allows the user to change the prompt string
*/
void setprompt(char **argv){
  printf("The prompt is now set to '%s'\n", argv[1]);
  prompt = argv[1];
}

/*
	this command allows the user to change directories
*/
void cd(char **argv){
  if(chdir(argv[1]) < 0){
    printf("There was an error in changing your directory. Please check the path name and retry.\n");
  }
}

/*
	this command prints a list of background jobs
*/
void bgjobs(){
  int i;
  for (i = 0; i < job_index; i++){
    if (pids[i] != 0 || background_jobs[i] != 0)
      printf("[%d] %s\n", job_numbers[i], background_jobs[i]);
  }
}

/*
	this command brings the specified background process to the foreground
*/
void fg(pid_t pid){
	int status;
	if (waitpid(pid, &status, 0) < 0){
		printf("balh errorororro\n");
	}
}

/*
	this command allows the user to dynamically link
	new commands from shared libraries
*/
void loadpluggin(char **argv){
  void *handle;
	char *error;
  struct NewBuiltIn *object_ptr;
	handle = dlopen(argv[1], RTLD_LOCAL | RTLD_LAZY); //open shared library
	if (!handle) { //check for error
 		fprintf(stderr, "%s\n", dlerror());
 		exit(1);
 	}
  object_ptr = dlsym(handle, "pluggin_method"); //link pluggin_method object
	commands[pluggin_index] = &(object_ptr->CommandName); //store command char array
  functions[pluggin_index] = &(object_ptr->FunctionName); //store function char array
  analyzers[pluggin_index] = &(object_ptr->AnalyzerName);//store analyzer char array
	//print pluggin_method char arrays
	printf("Command: %s\n", commands[pluggin_index]);
	printf("Function: %s\n", functions[pluggin_index]);
	printf("Analyzer: %s\n", analyzers[pluggin_index]);
	builtin_commands[builtin_index] = &(object_ptr->CommandName); //update the already built in commands into this array
	builtin_index++;
	function[pluggin_index] = dlsym(handle, functions[pluggin_index]); //store the pointer to function
	pluggin_index++;
	if ((error = dlerror()) != NULL) {//check for error
 		fprintf(stderr, "%c\n", *error);
 		exit(1);
 	}
}

/*
	this command exits the program
*/
void culater(){
  printf("CULATER Aligator\n");
  exit(0);
}

void file_redirection(char **argv){
	int f1; //initilize file descriptors
	int f2;
	int infile = 0; //flags for input/output
	int outfile = 0;
	int i;

	char input[BUFLEN]; //char arrays for file names
	char output[BUFLEN];

	while (argv[i] != '\0'){ //loop until end of cmd input is reached
		if (strcmp(argv[i], "<") == 0){
			argv[i] = NULL; //set token to null
			strcpy(input, argv[i+1]); //copy filename into input
			infile = 1; //check flag
		}
		else if (strcmp(argv[i], ">") == 0){
			argv[i] = NULL;
			strcpy(output, argv[i+1]); //copy filename into output
			outfile = 1;
		}
		i++;
	}
	if (infile){ //case we need to read from a file
		if ((f1 = open(input, O_RDONLY, 0)) < 0){
			printf("There was an error in opening your file\n");
			exit(0);
		}
		dup2(f1, STDIN_FILENO); //file redirection to new fd->STDIN_FILENO
		close(f1); //close file
	}
	if (outfile){
		if ((f2 = creat(output, 512)) < 0){
			printf("There was an error in creating a file to write to\n");
			exit(0);
		}
		dup2(f2, STDOUT_FILENO); //file redirection to new fd->STDOUT_FILENO
		close(f2);
	}
}
