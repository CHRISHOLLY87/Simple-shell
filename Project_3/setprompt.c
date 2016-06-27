#include <stdio.h>
#include <string.h>

struct NewBuiltIn{
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};

struct NewBuiltIn pluggin_method = {{"setprompt", "setprompt", ""}};



int setprompt(char *argv[]) {
	extern char *prompt;
	prompt = argv[1];
	return 0;
}
