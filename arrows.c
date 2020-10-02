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

void arrows(int redirect[], int argc, char argv[][SIZE]) {
	char ifile[50] = "", ofile[50] = "";
	int fd;
	bool append = false;
	char path[100];
	char **args = malloc(80 * sizeof(char *));

	redirect[0] = 0;
	redirect[1] = 1;
	// printf("1\n");
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i],">>")) {
			append = true;
			strcpy(ofile, argv[i+1]);
			strcpy(argv[i], "");
			strcpy(argv[i+1], "");
			i++;
		} else if (!strcmp(argv[i], ">")) {
			strcpy(ofile, argv[i+1]);
			strcpy(argv[i], "");
			strcpy(argv[i+1], "");
			i++;
		} else if (!strcmp(argv[i], "<")) {
			strcpy(ifile, argv[i+1]);
			strcpy(argv[i], "");
			strcpy(argv[i+1], "");
			i++;
		}
	}
	// for (int i = 0; i < argc; ++i)
	// 	printf("%s\n", argv[i]);

	if (strcmp(ifile, "")) {
		getcwd(path, 99);
		strcat(path, "/");
		strcat(path, ifile);
		// printf("i\n");
		// printf("%s\n", path);
		if ((fd = open(path, O_RDONLY)) < 0) {
			perror("open");
			exit(1);
		}
		redirect[0] = fd;
	}
	if (strcmp(ofile, "")) {
		getcwd(path, 99);
		strcat(path, "/");
		strcat(path, ofile);
		// printf("o\n");
		// printf("%s\n", path);

		if (append) {
			if ((fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0644)) < 0) {
				perror("open a");
				exit(1);
			}
			// printf("append\n");

		} else {

			if ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
				perror("open o");
				exit(1);
			}
		}
		redirect[1] = fd;
	} 
	// printf("3\n");
}
