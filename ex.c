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
#include "pipefun.h"

extern char process_name[50][25];
extern pid_t bg_pids[50];
extern int total;

bool bg;
void format(char *word) {
	int n = strlen(word);
	if (word[n-1] == '\n')
		word[n-1] = 0;
}

void zhandler() {
	bg = true;
	printf("now bg\n");
}

int ex(int argc, char* begin, char argv[][SIZE]) {
	//why not make sure only parent reaches here ?
	// if (pid) {
		// printf("one %s\n", getenv(argv[0]));
		int pid;
		for (int i = 0; i < argc; ++i) {
			// printf("%s\n", begin);
			if (strcmp(argv[i], "~") == 0)
				strcpy(argv[i], begin);
		}
		if (argc == 0)
			return false;
		else if (!strcmp(argv[0], "quit"))
			exit(0);
		else if (has_arrows(argc, argv))
			arrows(argc, argv);
		// else if (has_pipe(argc, argv))
			// pipefun(argc, argv);
		else if (argc == 1 && argv[0][0] == '$') {
			char *str =getenv(argv[0]+1);
			if (str != NULL)
				printf("%s\n", str);
		}
		else if (strcmp(argv[0], "cd") == 0) {
			cd(argc, begin, argv);
			return true;
		}
		else if (strcmp(argv[0], "ls") == 0) {
			char str[SIZE];
			getcwd(str, SIZE);
			ls(argc, argv);
			chdir(str);
			return true;
		}
		else if (strcmp(argv[0], "pwd") == 0) {
			pwd(argc, argv);
			return true;
		} 
		else if (strcmp(argv[0], "pinfo") == 0) {
			pinfo(argc, argv);
			return true;
		} else if (strcmp(argv[0], "echo") == 0) {
			for (int i = 1; i < argc; ++i)
				printf("%s ", argv[i]);
			printf("\n");
			return true;
		} else if (strcmp(argv[0], "history") == 0) 
			history(argc, argv);	
		else if  (strcmp(argv[0], "setenv") == 0)
			setenvfun(argc, argv); 
		else if (strcmp(argv[0], "unsetenv") == 0)
			unsetenvfun(argc, argv);
		else if (strcmp(argv[0], "jobs") == 0)
			jobs(argc, argv);
		else if (!strcmp(argv[0], "overkill"))
			overkill();
		else if (!strcmp(argv[0], "kjob"))
			kjob(argc, argv);
		else if (!strcmp(argv[0], "fg"))
			fg(argc, argv);
		else { 
			char* edit[argc+1];
			bool flag = 0;
			edit[argc] = NULL;
			for (int i = 0; i < argc; ++i)
				edit[i] = argv[i];
			signal(SIGCHLD, handler);
			signal(SIGTSTP, zhandler);
			if (strcmp(argv[argc-1], "&") == 0) {
				bg = true;
				edit[argc-1] = NULL;
			} else
				bg = false;
			// printf("system call %s\n", argv[0]);
			pid = fork();
			if (!pid) {
				// printf("ex child %d\n", getpid());
				// printf("child one %d\n", getpid());
				char **args;
				args = malloc(80 * sizeof(char *));
				args[0] = "cat";
				args[1] = "/proc/self/status";
				// execvp(args[0], args); 
				kill(getpid(), SIGCONT);
				if (bg)
					setpgid(0, 0);

				execvp(edit[0], edit);
				// execvp(args[0], args); 

				// signal(SIGTSTP, SIG_DFL);

				if (errno) {
					// printf("I am here\n");
					perror("ex execvp");
					kill(getpid(), SIGKILL);
				} 

			} else {
				// printf("ex parent %d\n", getpid());
				// printf("two %d\n", getpid());
				
				if (!bg) {
					int status;
					printf("I am waiting\n");
					
					pid = wait(&status);
					printf("stopped wait\n");
					if (WIFEXITED(status)) {
						//normal
						// printf("normal\n");
					} else {
						//abnormal
						// printf("abnormal\n");
					}
				}
				else {
					strcpy(process_name[total], edit[0]);
					bg_pids[total] = pid;
					total++;
				}
			}
		// }
	}
	
}