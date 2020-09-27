#include "headers.h"
#include "ex.h"


#define READ_END 0
#define WRITE_END 1



int pd[2];
void run(int argc, char argv[][SIZE]) {
	
	pipe(pd);
	if (!fork()) {
		dup2(pd[1], 1);
		ex(argc, argv[0], argv);
		// perror("exec");
		abort();
	}
	dup2(pd[0], 0);
	close(pd[1]);	
	

}

int main(int argc, char **argv) {
	int i;
	int index1=0;
	int index2=0;
	char args1[20][SIZE]; 
	char args2[50][SIZE];
	char commands[SIZE];
	fgets(commands, SIZE, stdin);
	printf(": %d %d\n", pd[0], pd[1]);
	//command 1 ; command 2;  
	char *command = strtok(commands, "|");
	while (command != NULL) {
		strcpy(args1[index1++], command);
		command = strtok(NULL, "|");
	}

	int n = strlen(args1[index1-1]);	
	args1[index1-1][n-1] = 0;

	// ls/-a ...;
	for (int i = 0; i < index1; ++i) {
		index2 = 0;
		char *word = strtok(args1[i], " ");
		while (word != NULL) {
			strcpy(args2[index2++], word);
			word = strtok(NULL, " ");		
		}
		char **args = malloc(80 * sizeof(char *));
		if (i != index1-1)
			run(index2, args2);		
		else
			ex(index2, args2[0], args2);

	}
	

	
	return 0;
}
