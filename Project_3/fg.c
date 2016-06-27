#include <stdio.h>

struct NewBuiltIn{
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};

struct NewBuiltIn pluggin_method = {{"fg", "fg", ""}};


int fg(char *argv[]){

  return 0;
}
