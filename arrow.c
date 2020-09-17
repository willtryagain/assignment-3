#include "headers.h"
#include "macros.h"

void arrows(int argc, char const argv[][SIZE]) {
	int pid;
	char line[81];
	char *token;
	char *separator = " \t\n"
	char **args, **args2;
	char *cp;
	char *ifile, *ofile;
	int i, j;
	int err;
	ifile = ofile = NULL;
	i = j = 0;
	err = 0;
	while (1) {
		cp = args[i++];
		if (cp == NULL)
			break;

		switch (*cp) {
			case '>':
				if (cp[1] == 0)
					cp = args[i++];
				else
					cp++;
				ofile = cp;
				if (cp == NULL)
					err = 1;
				else if (cp[0] == 0)
					err = 1;
				break;
			case '<':
				if (cp[1] == 0)
					cp = args[i++];
				else
					cp++;
				ifile = cp;
				if (cp == NULL)
					err = 1;
				else if (cp[0] == 0)
					err = 1;
				break;
			default:
				args2[j++] = cp;
				break;
		}
	}

	args2[j] = NULL;
	if (err || j == 0) 	{
		printf("err");
		continue;
	}			
	switch (pid = fork()) {
		case 0:
			//open stdin
			if (ifile != NULL) {
				int fd = open(ifile, O_RDONLY);

				if (dup2(fd, STDIN_FILENO) == -1) 
					fprintf(stderr, "dup2 failed");
				
				close(fd);
			}	
			//open stdout {}
			if (ofile != NULL) {
				int fd2;
				if ((fd2 = open(ofile, O_WRONLY | O_CREAT, 0644))) {
					perror("couldn't open output file.");
					exit(0);
				}
				dup2(fd2, STDOUT_FILENO);
				close(fd2);
			}

			execvp(args2[0], args2);
			signal(SIGINT, SIG_DFL);
			fprintf(stderr, "ERROR no such program\n");
			exit(1);
			break;
		case -1:
			fprintf(stderr, "ERROR can't create child process!\n");
            break;

          default:
           wait(NULL);
	}
	return 0;
}
