#include "headers.h"

void echo(int argc, char **argv) {
	for (int i = 1; i < argc; ++i)
		printf("%s ", argv[i]);
	printf("\n");
	// for (int i = 0; i < total; ++i)
	// 	printf("%s\n", process_name[i]);
}