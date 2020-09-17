#include "headers.h"
#include "macros.h"

void setenv_fun(int argc, char argv[][SIZE]) {
	if (argc == 1 || argc > 3) {
		printf("setenv: invalid count of arguments\n");
		return;
	} else if (argc == 2) 
		setenv(argv[1], "", 1);
	else 
		setenv(argv[1], argv[2], 1);
	
}
