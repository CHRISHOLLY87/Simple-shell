#include <stdio.h>

struct NewBuiltIn{
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};

struct NewBuiltIn pluggin_method = {{"comment", "comment", ""}};

int comment() {
	printf("You entered a comment...");
	return 0;
}
