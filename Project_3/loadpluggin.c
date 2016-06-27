#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

struct NewBuiltIn{
	char *CommandName[64];
	char *FunctionName[64];
	char *AnalyzerName[64];
};

struct NewBuiltIn pluggin_method = {{"loadpluggin", "loadpluggin", ""}}; //cite bug http://stackoverflow.com/questions/11551986/gcc-missing-braces-around-initializer

extern char *builtin_functions[64];
extern void *pointers_to_functions[64];
extern int ptr_to_func_index;

/*
  the purpose of the loadpluggin function is to append the pointer to a function
  to a data structure defined as a global variable
*/
int loadpluggin(char *argv[]){
  //append argv[1] to builtin_commands
  int *handle;
  struct NewBuiltIn *object_ptr;
  void *ptr_to_func;
  handle = dlopen(argv[1], RTLD_LOCAL | RTLD_LAZY); //open
  object_ptr = dlsym(handle, "pluggin_method"); //return pluggin_method into a pointer to a function
  ptr_to_func = dlsym(handle, *object_ptr->FunctionName); //return the name of the function
	pointers_to_functions[ptr_to_func_index] = ptr_to_func;
	builtin_functions[ptr_to_func_index] = *object_ptr->FunctionName;
	ptr_to_func_index++;
  return 0;
}
