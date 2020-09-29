#include "headers.h"
#include "macros.h"
#include "prompt.h"
#include "process_args.h"
#include "signal.h"
#include "old.h"
#include "ex.h"

extern bool bg;
pid_t bg_pids[50];
int total = 0;
char process_name[50][25];

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
	get_begin(begin);
	while (1) {
		// for (int i = 0; i < total; ++i)
		// 	fprintf(stderr, "%s/", process_name[i]);
		
		prompt(begin);
		// perror("main");
		if (fgets(str, SIZE, stdin) == NULL)
			exit(0);
		save(str);
		signal(SIGINT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		
		split_commands(str, begin);

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
