#include "headers.h"
#include "macros.h"
#include "ex.h"
#include "pwd.h"


// extern int pid;
// extern bool bg;

static int cnt = 0;
int pd[2];
void run(int argc, char argv[][SIZE]) {
	int pid=-1;
	pipe(pd);
	char **args = malloc(80 * sizeof(char *));
    for (int i = 0; i < argc; ++i)
        args[i] = strndup(argv[i], 100);
    args[argc] = NULL;
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

		if ((filedes[0] = dup(0)) < 0)
			perror("dup 1");
		if ((filedes[1] = dup(1)) < 0)
			perror("dup ii");

		for (int j = 0; j < index2; ++j) {
			index3 = 0;
			char *word = strtok(args2[j], " ");
			
			while (word != NULL) {
				strcpy(args3[index3++], word);
				word = strtok(NULL, " ");		
			}
			// char **args = malloc(80 * sizeof(char *));
			strcpy(temp[0], "ls");
			// cnt++;
			// if (cnt == 1)
			// 	run(1, temp);

			if (j != index2-1)
				run(index3, args3);		
			else
				ex(index3, args3[0], args3);
			printf("\n");
		}
		

		// ex(1, temp[0], temp);
		// ex(1, temp[0], temp);
		if (dup2(filedes[0], 0) < 0) {
			perror("dup2 i");
			exit(1);
		}
		if (dup2(filedes[1], 1) < 0) {
			perror("dup2 ii");
			exit(1);
		}
	}
	
	
}