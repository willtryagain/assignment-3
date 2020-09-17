#include "prompt.h"
#include "headers.h"
#include "macros.h"

int find_name(int pid, char *name) {
	extern int total;
	extern char process_name[50][SIZE];
	extern pid_t bg_pids[50];
	for (int i = 0; i < total; ++i) {
		if (pid == bg_pids[i]) {
			strcpy(name, process_name[i]);
			return 1;
		}
	}
	// printf("pid not found\n");
	return 0;
}

void handler(int status) {
	char name[SIZE];
	char type[50];
	// printf("I am wait s\n");
	pid_t pid = waitpid(-1, &status, WNOHANG);
	// printf("wait s os\n");
	// pid = wait(&status);
	if (WIFEXITED(status)) {
		//normal
		strcpy(type, "normally");
	} else {
		//abnormal
		strcpy(type, "abnormally");
	}
	if (find_name(pid, name))
		fprintf(stderr, "%s with pid %d exited %s\n", name, pid, type);
}

