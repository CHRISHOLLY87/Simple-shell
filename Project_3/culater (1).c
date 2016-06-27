#include <stdio.h>

struct NewBuiltIn{
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};

struct NewBuiltIn pluggin_method = {{"culater", "culater", ""}};

int culater(){
  return 0;
}
