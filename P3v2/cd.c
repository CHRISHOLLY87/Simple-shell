#include <stdio.h>
#include <unistd.h>

struct NewBuiltIn{
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};

struct NewBuiltIn pluggin_method = {{"cd", "cd", ""}};

int cd(char *argv[]) {
	if(chdir(argv[1]) < 0){
		printf("There was an error in changing your directory. Please check the path name and retry.\n");
	}
	return 0;
}

void cd_analyzer(){

}
