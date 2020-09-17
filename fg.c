#include "headers.h"
#include "macros.h"

void fg(int argc, char argv[][SIZE]) {
	extern pid_t bg_pids[50];
	extern int total;
	int job_number = atoi(argv[1]);
	// printf("%d %d\n", job_number, signal_number);
	if (job_number > total) 
		printf("Number is not present\n");

	else if (kill(bg_pids[job_number-1], SIGCONT) < 0)
		perror("fg");
}