#include "headers.h"
#include "macros.h"
#include "ex.h"
#include "pwd.h"

//fd of pipe
int pd[2] = {0, 1};

void run(int n, int index, int argc, char argv[][SIZE]) {
	pid_t pid;
	if (pipe(pd) < 0) {
		perror("pipe");
		exit(1);
	}
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	if ((pid = fork()) == 0) {
		if (index != n-1) {
			close(1);
			dup(pd[1]);
		}
		execvp(argv[0], argv);
		// ex(argc, argv[0], argv);

	} else {
		sleep(1);
		if (index != n-1) {
			close(0);	
			dup(pd[0]);
		}
		kill(pid, SIGTERM);
	}
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
	int len, index1=0, index2=0, index3=0;
	int count;
	char args1[11][SIZE]; 
	char args2[50][SIZE];
	char args3[50][SIZE], temp[50][SIZE];
	int filedes[2] = {0, 1};
	
	/*
	separate command 1 ; command 2;  
 	to command1, command2,...
	*/
	char *command = strtok(commands, ";");
	while (command != NULL) {
		strcpy(args1[index1++], command);
		command = strtok(NULL, ";");
	}

	//eliminate the '\n'
	int n = strlen(args1[index1-1]);	
	args1[index1-1][n-1] = 0;

	/*
	Separate command1 | command 2| command3... 
	*/
	for (int i = 0; i < index1; ++i) {
		index2 = 0;
		char *word = strtok(args1[i], "|");
		while (word != NULL) {
			strcpy(args2[index2++], word);
			word = strtok(NULL, "|");		
		}
		/*
		Separate a b c d ..
		to a, b, c, ...
		*/
		for (int j = 0; j < index2; ++j) {
			char *word = strtok(args2[j], " ");
			index3 = 0;
			while (word != NULL) {
				strcpy(args3[index3++], word);
				word = strtok(NULL, " ");		
			}
			run(index2, j, index3, args3);		
		}
	}
	
	
}