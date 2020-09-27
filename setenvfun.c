#include "headers.h"
#include "macros.h"

void setenvfun(int argc, char** argv) {
	if (argc == 1 || argc > 3) {
		printf("setenv: invalid count of arguments\n");
		return;
	} else if (argc == 2) {
		setenv(argv[1], "", 1);
		// printf("%s\n", getenv(argv[1]));
	}
	else {
		setenv(argv[1], argv[2], 1);
	}
	if (errno)
		perror("env");
	
	
}
