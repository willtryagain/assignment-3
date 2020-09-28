#include "headers.h"
#include "macros.h"
#include "ex.h"
#include "pwd.h"


// extern int pid;
// extern bool bg;

static int cnt = 0;
int pd[2] = {0, 1};
void run(int n, int index, int argc, char argv[][SIZE]) {
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	if (pid == 0) {
		if (index == 0)
			close(pd[0]);
		else {
			dup2(pd[0], 0);
			close(pd[])
		}

	}
	if ((pid = fork()) == 0) {
		dup2(pd[1], 1);
		// execvp(args[0], args);
		ex(argc, argv[0], argv);
		// perror("exec1");
  //       abort();

	} else {
		close(pd[1]);	
		sleep(1);
		kill(pid, SIGTERM);
	}
	dup2(pd[0], 0);
	

}

void get_args(char *command, int *count, char args[][SIZE]) {
	int cnt = 0;
	char *word = strtok(command, " ");
	while (word != NULL) {
		format(word);
		strcpy(args[++cnt], word);
		word = strtok(NULL, " ");
	}
	*count = cnt;
}


void split_commands(char *commands, char *begin) {	
	int type = -1;
	int len, index1=0, index2=0, index3=0;
	int count;
	char args1[11][SIZE]; 
	char args2[50][SIZE];
	char args3[50][SIZE], temp[50][SIZE];
	int filedes[2] = {0, 1};
	
	//command 1 ; command 2;  
	char *command = strtok(commands, ";");
	while (command != NULL) {
		strcpy(args1[index1++], command);
		command = strtok(NULL, ";");
	}

	int n = strlen(args1[index1-1]);	
	args1[index1-1][n-1] = 0;

	// ls/-a ... 
	for (int i = 0; i < index1; ++i) {
		index2 = 0;

		char *word = strtok(args1[i], "|");
		while (word != NULL) {
			strcpy(args2[index2++], word);
			word = strtok(NULL, "|");		
		}
		pipe(pd);
		for (int j = 0; j < index2; ++j) {
			char *word = strtok(args2[j], " ");
			index3 = 0;
			while (word != NULL) {
				strcpy(args3[index3++], word);
				word = strtok(NULL, " ");		
			}
			run(index2, j, index3, args3);		
			else {
				int pid;
				if ((pid = fork()) < 0) {
					perror("could not fork!\n");
					exit(1);
				} 
				if (!pid) {
					// pipe(pd);
					dup2(pd[1], 0);
					ex(index3, args3[0], args3);
					// execvp(args[0], args);
				} 
				wait(NULL);
	
				// kill(getpid(), SIGKILL);
			}
			// printf("\n");
		}
		

		// ex(1, temp[0], temp);
		// ex(1, temp[0], temp);
	}
	
	
}