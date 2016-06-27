#include <stdio.h>
#include <unistd.h>

struct NewBuiltIn{
	char *CommandName[64] = "cd";
	char *FunctionName[64] = "cd";
	char *AnalyzerFunction[64] = "cd_analyzer";
};

int cd(char *argv[]) {
	if(chdir(argv[1]) < 0){
		printf("There was an error in changing your directory. Please check the path name and retry.\n");
	}
	return 0;
}

int cd_analyzer(){

}
