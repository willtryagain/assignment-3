#include "headers.h"
#include "macros.h"
#include "cd.h"
#include "lsall.h"
#include "pinfo.h"
#include "pwd.h"
#include "old.h"
#include "signal.h"
#include "setenvfun.h"
#include "unsetenvfun.h"
#include "arrows.h"
#include "jobs.h"
#include "overkill.h"
#include "kjob.h"
#include "fg.h"
#include "bg.h"
#include "pipefun.h"
#include "echo.h"

int pid;
bool is_bg;
char name[50];

extern char process_name[50][25];
extern pid_t bg_pids[50];
extern int total;

void format(char *word) {
	int n = strlen(word);
	if (word[n-1] == '\n')
		word[n-1] = 0;
}

void zhandler() {
	int child_id;
	if ((child_id = fork()) < 0) {
		perror("could not fork!\n");
		exit(1);
	} 
	if (child_id) 
		kill(pid, SIGSTOP);
	strcpy(process_name[total], name);
	bg_pids[total++] = pid;

	
}

void chandler() {
	// printf("int\n");
	if (kill(getpid(), SIGINT) < 0)
		perror("ctrlc");
} 



char **set_argv() {
	char **argv = malloc(80 * sizeof(char *));
	for (int i = 0; i < 80; ++i)
		argv[i] = NULL;
	return argv; 
}



void ex(int argc, char* begin, char args[][SIZE]) {
	char **argv = set_argv();
	int filedes[2] = {0, 1};
	int redirect[2] = {0, 1};
	int cnt = 0;
	// int pid;
	char *str;
	is_bg = false;
	// fprintf(stderr, "c%d\n", getpid());
	if ((filedes[0] = dup(0)) < 0) 
		perror("dup 1");
	if ((filedes[1] = dup(1)) < 0)
		perror("dup ii");
	
	if (argc == 0)
		return;

	if (has_arrows(argc, args)) {
		arrows(redirect, argc, args);
		// printf("%d %d\n", redirect[0], redirect[1]);
		if (dup2(redirect[0], 0) < 0) {
			perror("dup2 i");
			exit(1);
		}
		if (dup2(redirect[1], 1) < 0) {
			perror("dup2 ii");
			exit(1);
		}
		// printf("%d %d\n", redirect[0], redirect[1]);
	}
	// fprintf(stderr, "> %d %d\n", filedes[0], filedes[1]);
	for (int i = 0; i < argc; ++i) {
		if (strcmp(args[i],"")) {
			if (!strcmp(args[i], "~"))
				strcpy(args[i], begin);
			argv[cnt++] = strndup(args[i], SIZE);
		}
	}
	argc =  cnt;
	argv[argc] = NULL;

	//command block
	//argv[0] = NULL || 
	if (!strcmp(argv[0], "quit"))
		exit(0);
	
	// else if (has_pipe(argc, argv))
		// pipefun(argc, argv);
	else if (argc == 1 && argv[0][0] == '$') {
		char *str =getenv(argv[0]+1);
		if (str != NULL)
			printf("%s\n", str);
	}
	else if (!strcmp(argv[0], "cd"))
		cd(argc, begin, argv);
	else if (!strcmp(argv[0], "ls"))
		ls(argc, argv);
	else if (!strcmp(argv[0], "pwd")) 
		pwd(argc, argv); 
	else if (!strcmp(argv[0], "pinfo"))
		pinfo(argc, argv);
	else if (!strcmp(argv[0], "echo")) 
		echo(argc, argv);
	else if (!strcmp(argv[0], "history")) 
		history(argc, argv);	
	else if  (!strcmp(argv[0], "setenv"))
		setenvfun(argc, argv); 
	else if (!strcmp(argv[0], "unsetenv"))
		unsetenvfun(argc, argv);
	else if (!strcmp(argv[0], "jobs"))
		jobs(argc, argv);
	else if (!strcmp(argv[0], "overkill"))
		overkill();
	else if (!strcmp(argv[0], "kjob"))
		kjob(argc, argv);
	else if (!strcmp(argv[0], "fg"))
		fg(argc, argv);
	else if (!strcmp(argv[0], "bg"))
		bg(argc, argv);

	//fork block
	else { 
		bool flag = 0;
		strcpy(name, argv[0]);
		if (!strcmp(argv[argc-1], "&")) {
			is_bg = true;
			argv[argc-1] = NULL;
			signal(SIGCHLD, handler);
		} 
		// signal(SIGTTOU, SIG_IGN);
		

		if ((pid = fork()) < 0) {
			perror("could not fork!\n");
			exit(1);
		}
		if (!pid) {
			signal(SIGTSTP, zhandler);
			signal(SIGINT, chandler);

			if (is_bg) {
				if (setpgid(0, 0) < 0) {
					perror("setpgid");
					exit(1);
				}
			}
			if (execvp(argv[0], argv) < 0) {
				perror("ex execvp");
				exit(1);
			}
			if (is_bg)
				pause();
		} 
		else {
			signal(SIGTSTP, zhandler);

			if (!is_bg)  {
				// printf("waiting\n");
				wait(NULL);

			}

			else {
				setpgid(pid, pid);
				strcpy(process_name[total], argv[0]);
				bg_pids[total++] = pid;
			}
		}

		
	}
	// for (int i = 0; i < total; ++i)
	// 	fprintf(stderr, "%s/", process_name[i]);
	// fprintf(stderr, "exit %d\n", is_bg);
	if (dup2(filedes[0], 0) < 0) {
		perror("dup2 i");
		exit(1);
	}
	if (dup2(filedes[1], 1) < 0) {
		perror("dup2 ii");
		exit(1);
	}
	
}