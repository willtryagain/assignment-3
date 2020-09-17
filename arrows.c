#include "headers.h"
#include "macros.h"

bool has_arrows(int argc, char argv[][SIZE]) {
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "<") || !strcmp(argv[i], ">") ||
			!strcmp(argv[i], ">>"))
			return true;
	}
	return false;
}

void arrows(int argc, char argv[][SIZE]) {
	int pid;
	char line[81];
	char *token;
	char *separator = " \t\n";
	char **args;
	char **args2;
	char *cp;
	char *ifile, *ofile;
	int i, j;
	int err;
	bool append = false;
	ifile = ofile = NULL;
    args2 = malloc(80 * sizeof(char *));


	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i],">>")) {
			append = true;
			ofile = argv[i+1];
			i++;
		} else if (!strcmp(argv[i], ">")) {
			ofile = argv[i+1];
			i++;
		} else if (!strcmp(argv[i], "<")) {
			ifile = argv[i+1];
			i++;
		}
	}

	args2[0] = argv[0];
	args2[1] =NULL;
	switch (pid = fork()) {
		case 0:
			//open stdin
			if (ifile != NULL) {
				int fd = open(ifile, O_RDONLY);
				printf("i - %s\n", ifile);
				if (dup2(fd, STDIN_FILENO) < 0) {
					fprintf(stderr, "dup2 i failed");
					exit(1);
				}
				
				close(fd);
			}	
			//open stdout {}
			if (ofile != NULL) {
				int fd2;
				printf("o - %s\n", ofile);
				if (append) {
					if ((fd2 = open(ofile, O_APPEND | O_CREAT, 0644)) < 0) {
						perror("couldn't open output file.");
						exit(1);
					}
				} else {
					if ((fd2 = open(ofile, O_WRONLY | O_CREAT, 0644)) < 0) {
						perror("couldn't open output file.");
						exit(1);
					}
				} 

				if (dup2(fd2, STDOUT_FILENO) < 0) 
					fprintf(stderr, "dup2 o failed");
				close(fd2);
			}
			// execvp(args2[0], args2);
			signal(SIGINT, SIG_DFL);
			// fprintf(stderr, "ERROR no such program\n");
			// exit(1);
			break;
		case -1:
			fprintf(stderr, "ERROR can't create child process!\n");
            break;

        default:
           wait(NULL);
	}
}
