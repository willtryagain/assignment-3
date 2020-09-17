#include "headers.h"

int main() {
	char *args[] = {"readlink", "/proc/self/exe", NULL};
	// char *args[] = {"vi"};
	execvp(args[0], args);

	printf("end");
	return 0;
}