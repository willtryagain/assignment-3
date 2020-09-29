#include "headers.h"
#include "macros.h"

void bg(int argc, char** argv) {
	extern pid_t bg_pids[50];
	extern int total;
	int job_number = atoi(argv[1]);
	// printf("%d %d\n", job_number, signal_number);
	if (job_number > total || total == 0) 
		printf("bg: no such job\n");

	else if (kill(bg_pids[job_number-1], SIGCONT) < 0)
		perror("bg");
}