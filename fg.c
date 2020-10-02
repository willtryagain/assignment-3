#include "headers.h"
#include "macros.h"

void fg(int argc, char** argv) {
	extern char process_name[50][25];
	extern pid_t bg_pids[50];
	extern int total;
	if (argv[1] == NULL) {
		fprintf(stderr, "fg: invalid argument");
		return;
	}
	int job_number = atoi(argv[1]);
	// printf("%d %d\n", job_number, signal_number);
	if (job_number > total || total <= 0) {
		fprintf(stderr, "fg: no job\n");
		return;
	}
	if (!fork()) {
		char **args = malloc(80 * sizeof(char *));
		args[0] = process_name[job_number-1];
		args[1] = NULL;
		execvp(args[0], args);
	}
	else 
		wait(NULL);

	// signal(SIGTTOU, SIG_IGN);
	// tcsetpgrp(STDIN_FILENO, bg_pids[job_number-1]);
	// signal(SIGTTOU, SIG_IGN);
	// if (kill(bg_pids[job_number-1], SIGCONT) < 0)
		// perror("fg");
	// sleep(2);
}