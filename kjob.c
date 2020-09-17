#include "headers.h"
#include "macros.h"

void kjob(int argc, char argv[][SIZE]) {
	extern pid_t bg_pids[50];
	extern int total;
	int job_number = atoi(argv[1]);
	int signal_number = atoi(argv[2]);
	printf("%d %d\n", job_number, signal_number);
	if (job_number > total) 
		printf("Number is not present\n");

	else if (kill(bg_pids[job_number-1], signal_number) < 0)
		perror("kjob");
}