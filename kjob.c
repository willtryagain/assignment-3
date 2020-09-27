#include "headers.h"
#include "macros.h"

void kjob(int argc, char** argv) {
	extern pid_t bg_pids[50];
	extern int total;
	if (argv[1] == NULL) {
		fprintf(stderr, "kjob: invalid argument");
		return;
	}
	int job_number = atoi(argv[1]);
	if (argv[2] == NULL) {
		fprintf(stderr, "kjob: invalid argument");
		return;
	}
	int signal_number = atoi(argv[2]);
	// printf("%d %d\n", job_number, signal_number);
	if (job_number > total) 
		printf("kjob: no job\n");

	else if (kill(bg_pids[job_number-1], signal_number) < 0)
		perror("kjob");
}