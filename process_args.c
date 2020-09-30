#include "headers.h"
#include "macros.h"
#include "ex.h"
#include "pwd.h"

//fd of pipe
int pd[2];

void run(int argc, char **argv) {
    pipe(pd);
    // printf("r\n");
    if (!fork()) {
        dup2(pd[1], 1);
        // fprintf(stderr, "%d %d\n", pd[0], pd[1]);
        execvp(argv[0], argv);
        perror("exec");
        abort();
    } else
    	close(pd[1]);  
    dup2(pd[0], 0);
}

bool found_pipe(char *str) {
	for (int i = 0; i < strlen(str); ++i) {
		if (str[i] == '|')
			return true;
	}
	return false;
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
	bool has_pipe = false;
    // printf("a");
    // printf("%d\n", STDIN_FILENO);

	/*
	separate command 1 ; command 2;  
 	to command1, command2,...
	*/
	char *command = strtok(commands, ";");
	while (command != NULL) {
		strcpy(args1[index1++], command);
		if (found_pipe(command)) 
			has_pipe = true;
		command = strtok(NULL, ";");
	}

	//eliminate the '\n'
	int n = strlen(args1[index1-1]);	
	args1[index1-1][n-1] = 0;

	if (has_pipe) {	
		// printf("p\n");
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
				char **args = malloc(80 * sizeof(char *));
		        for (int i = 0; i < index3; ++i)
		            args[i] = strndup(args3[i], 100);
		        args[index3] = NULL;
		        if (j != index2-1)
		            run(index3, args);      
		        else {
		        	printf("hundred\n");
		        	dup2(pd[0], 0);
		            execvp(args[0], args);		
		        }
			}
		}
		// kill(getpid(), SIGKILL);
	}
	else {
		for (int i = 0; i < index1; ++i) {
			index2 = 0;
			char *word = strtok(args1[i], " ");
			while (word != NULL) {
				strcpy(args2[index2++], word);
				word = strtok(NULL, " ");		
			}
			ex(index2, begin, args2);
		}
	}
	
}