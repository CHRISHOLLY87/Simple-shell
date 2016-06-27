/*
  UserMore puggin
  Kendall Weihe
  Steve Pearce
  CS 485G Fall 2015 Griffieon
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

struct NewBuiltIn {
    char CommandName[64];   /* Name of the Built-in command users can type */
    char FunctionName[64];  /* Name of the function in the code */
    char AnalyzerName[64];  /* Name of an analyzer function to call on every command */
};

struct NewBuiltIn pluggin_method = {
	"usermore",
	"user_more"
};

//for best results execute stty -echoctl prior to executing plugin

//CITE http://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
void sig_int(int); //initialize signal handler function

static volatile int keepRunning = 0; //initialize the signal variable -- to be checked in if-condition

int user_more(char *argv[]){

    printf("This program acts like the cat command from the UNIX OS\n");
    printf("  ...only it prints out the first x lines that you specify\n");
    printf("  ...after this to print out the next x lines, press 'CTRL-\'\n");
    printf("For cleanest results, enter command `stty -echoctl' prior to running the plugin\n");

    //CITE http://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
    char buf[1024]; //buffer to hold the input file data
    FILE *file; //file descriptor
    int i; //for loops index
    int j = 1; //integer to print the line number
    int k = 0; //flag to print first x lines
    int number_of_lines;

    printf("Enter the number of lines you want to diplay: "); //input number of lines to print
    scanf("%d", &number_of_lines);

    file = fopen(argv[1], "r"); //open file
    if (file) {
        while (fgets(buf, sizeof(buf), file) != NULL){ //input first line of file and loop until the end of the file
          if (k == 0){
            //print out first x lines
            for (i = 0; i < number_of_lines; i++){
              printf("Line %d: %s", j, buf);
              fgets(buf, sizeof(buf), file); //get next line input
              j++; //increment line number
            }
          k++;
          }
          signal(SIGQUIT, sig_int); //handle CTRL-\ signal
          pause(); //pause and wait for signal
          if (keepRunning){ //if the user inputs 'CTRL-\'
            for (i = 0; i < number_of_lines; i++){
              printf("Line %d: %s", j, buf);
              j++; //increment line number
                if (fgets(buf, sizeof(buf), file) == NULL){ //exit program once the end of file is reached
                  fclose(file); //close the file descriptor table
                  return 0;
                }
            }
            keepRunning = 0; //reset signal variable flag to 0
      }
    }
  }
  return -1;
}

void sig_int(int sig){
  keepRunning = 1;
}

