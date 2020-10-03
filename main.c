#include "headers.h"
#include "macros.h"
#include "prompt.h"
#include "process_args.h"
#include "signal.h"
// #include "old.h"
#include "ex.h"
#include "restore_fd.h"


extern bool bg;
pid_t bg_pids[50];
int total = 0;
char process_name[50][25];
int counter_ = 0;
struct stat st;

void get_begin(char *begin) {
	char cwd[100];
	getcwd(cwd, 100);
	if (errno) {
		perror("working directory main");
		return;
	}
	strcpy(begin, cwd);
}

int main() {
	char begin[500], str[100];
	int fds[2] = {0, 1};
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	get_begin(begin);
	save(fds);
	while (1) {
		// for (int i = 0; i < total; ++i)
		// 	fprintf(stderr, "%s/", process_name[i]);
		fstat(0, &st);
		prompt(begin);
		// printf("%d %d %d\n", getpid(), counter_++, st.st_ino);
		if (fgets(str, SIZE, stdin) == NULL)
			exit(0);
		

		split_commands(str, begin);
		restore(fds);
	}
	// printf("out of loop\n");
	// if (waitpid(parent, &wstatus, WNOHANG) == -1) 
 //    	perror("waitpid");
    // waitpid(parent, &wstatus, WNOHANG);
    // if (WIFEXITED(wstatus)) {
    // 	es = WEXITSTATUS(wstatus);
    // 	printf(" with pid %d\n exited normally", parent);
    // 	printf("Exit status was %d\n", es);
    // }
    // if (foreground)
    // 	wait(NULL);

	return 0;
}
