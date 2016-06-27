/*
	Big Blue Tester pluggin
	Kendall Weihe
	Steve Pearce
	CS485G Fall 2015 Griffieon
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct NewBuiltIn {
    char CommandName[64];   /* Name of the Built-in command users can type */
    char FunctionName[64];  /* Name of the function in the code */
    char AnalyzerName[64];  /* Name of an analyzer function to call on every command */
};

struct NewBuiltIn pluggin_method = {
	"bigbluetest",
	"big_blue_test"
};

/*
	function: big_blue_test
	inputs: user answer to question
	outputs: question and result
	purpose: this program is a simple game to test the users knowledge
						of the University of Kentucky basketball team.
						the program asks the user what a random players number is,
						and the user must answer.
						the program then outputs if the user was correct or not.
						once the user decides to quit the game, the program outputs
						how many questions the user answered correctly.1
*/
int big_blue_test(){
	//output opening statements and ask the user if they are ready to begin
	printf("Welcome to the Big Blue Test,\n this program will test your knowledge of the University of Kentucky Wildcats 2015-2016 basketball roster\n\n");
	printf("Type ready or r to begin: ");
	char ready[16];
	fgets(ready, 16, stdin);
	//initialize the roster array
	char *roster[2][14] = {
		{"13", "10", "24", "25", "15", "1", "00", "4", "11", "23", "22", "32", "3", "35"},
		{"Isaiah Briscoe", "Jonny David", "EJ Floreal", "Dominique Hawkins", "Isaac Humphries", "Skal Labissiere", "Marcus Lee", "Charles Matthews", "Mychal Mulder", "Jamal Murray", "Alex Poythress", "Dillon Pulliam", "Tyler Ulis", "Derrick Willis"}
	};
	int random;
	char answer[128];
	
	int correct_count = 0;
	
	if (strcmp(ready, "ready") || strcmp(ready, "r")){
		//begin the test
		printf("Enter 'q' or 'quit' to stop at any point\n");
		srand (time(NULL)); //reset rand()
		while(1){
				random = rand() % 14; //generate random question
				printf("What is %s's number?  ", roster[1][random]); //ask the user the question
				fgets(answer, 4, stdin); //take users answer
				strtok(answer, "\n"); //splice off the trailing newline character that fgets appends for strcmp
				if (!strcmp(answer, "q") || !strcmp(answer, "quit")){
					printf("You answered [ %d ] questions correctly... CATS BY 90!\n", correct_count);
					return 0;
				}
				if (!strcmp(answer, roster[0][random])){
					//correct
					printf("GO BIG BLUE! You were correct!\n");
					correct_count++;
				}
				else {
					//incorrect
					printf("Brush up on your basketball knowledge, your team needs you\n");
				}
		}
	}
	else{
		printf("Sorry your input was not recognized, please rerun the program\n");
		return -1;
	}
	return 0;
}
