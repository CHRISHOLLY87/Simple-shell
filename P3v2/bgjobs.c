#include <stdio.h>
#include <unistd.h>

struct NewBuiltIn{
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};

struct NewBuiltIn pluggin_method = {{"bgjobs", "bgjobs", ""}};

int bgjobs(pid_t pid[]) {
	//i need to find the index of the current background process
	//do we wait for the background jobs to finish?
	//this way we can always be dynamically updating the background_jobs array
	int i;
	for(i = 0; i < sizeof(pid); i++){
		printf("[%d] <names>", pid[i] );
	}
	return 0;
}
