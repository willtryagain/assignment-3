#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int SIZE = 1024;
	int wstatus;
	int es;
	char str[SIZE];
	int id = fork();
	if (id == 0) {
		char *args[] = {"chromium-browser", NULL};
  		printf("\n");
  		execvp(args[0], args);
	} 
	while (1) {
		if (id) {
			printf("Parent\n");
			printf("$");
		    fgets(str, SIZE, stdin);
		    if (waitpid(id, &wstatus, WNOHANG) == -1) 
		    	perror("waitpid");
		    // printf("1");
		    else if (WIFEXITED(wstatus)) {
		    	es = WEXITSTATUS(wstatus);
		    	printf("%d\n", id);
		    	printf("Exit status was %d\n", es);
		    }
		} else {
			printf("child\n");
		}
	}

	
	
}