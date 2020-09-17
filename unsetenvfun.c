#include "headers.h"
#include "macros.h"

void unsetenvfun(int argc, char argv[][SIZE]) {
	if (argc != 2) {
		printf("unsetenv: invalid count of arguments\n");
		return;
	} 
	else 
		unsetenv(argv[1]);
		// printf("%s\n", getenv(argv[1]));
	
	if (errno) 
		perror("uenv");
	
}
