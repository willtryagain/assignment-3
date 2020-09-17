#include "headers.h"
#include "macros.h"
#include "ex.h"
#include "pwd.h"


extern int pid;
extern bool bg;

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
	int len, index1=0, index2=0;
	int count;
	char args1[11][SIZE]; 
	char args2[50][SIZE];
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
		char *word = strtok(args1[i], " ");
		while (word != NULL) {
			strcpy(args2[index2++], word);
			word = strtok(NULL, " ");		
		}

		// pid = 0; // ??
		// bg = 1;	// ??
		ex(index2, begin, args2);
		// printf("pid: %d bg %d\n", getpid(), bg);
		// printf("after ex should be parent %d\n", getpid());
		// if (pid && !bg) 
		// 	wait(NULL);
	}
	
}