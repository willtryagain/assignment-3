#include "headers.h"
#include "macros.h"

void fg(int argc, char** argv) {
	extern pid_t bg_pids[50];
	extern int total;
	if (argv[1] == NULL) {
		fprintf(stderr, "fg: invalid argument");
		return;
	}
	int job_number = atoi(argv[1]);
	// printf("%d %d\n", job_number, signal_number);
	if (job_number > total || total == 0) {
		fprintf(stderr, "fg: no job\n");
		return;
	}

	// signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(STDIN_FILENO, getpid());
	if (kill(bg_pids[job_number-1], SIGCONT) < 0)
		perror("fg");
	sleep(2);
}