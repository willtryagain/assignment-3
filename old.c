#include "headers.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "macros.h"

int min(int a, int b) {
	if (a < b)
		return a;
	return b;
}

int find_count(FILE *f) {
	int lines = 0;
	char ch;
	while (true) {
		if (feof(f))
			break;
		ch = fgetc(f);
		if (ch == '\n')
			lines++;
	}
	fclose(f);
	return lines;
}

void remove_first() {
	int line = 0;
	size_t len = 0;
	char strings[100][SIZE];
	char *str = NULL;
	FILE *f = fopen("history.txt", "r");
	if (errno) {
		perror("history fopen remove");
		exit(0);
	}
	while (getline(&str, &len, f) != -1) {
		strcpy(strings[line++], str);
	}
	
	fclose(f);
	// printf("%d", line);
	f = fopen("history.txt", "w");
	for (int i = line-20; i < line; ++i) {
		fprintf(f, "%s", strings[i]);
		fflush(f);

	}
	fclose(f);
}

void save(char *str) {
	open("history.txt", O_RDONLY | O_WRONLY | O_CREAT, 0644);
	if (errno) {
		perror("history fopen save");
		exit(0);
	}
	FILE *f = fopen("history.txt", "r");
	int lines = find_count(f);
	if (lines >= 20) 
		remove_first();
	FILE *fb = fopen("history.txt", "a");
	fprintf(fb, "%s", str);
	fflush(fb);
	if (errno) 
		perror("printf");
}

void history(int argc, char ** argv) {
	int line = 0;
	size_t len = 0;
	char strings[100][SIZE];
	char *str = NULL;
	// for (int i = 0; i < argc; ++i)
	// 	printf("%s\n", argv[i]);
	open("history.txt", O_RDONLY | O_WRONLY | O_CREAT, 0644);
	if (errno) {
		perror("history fopen history");
		exit(0);
	}
	FILE *f = fopen("history.txt", "r");
	if (errno) {
		perror("history fopen get_info");
		exit(0);
	}
	// printf("h\n");
	while (getline(&str, &len, f) != -1)  {
		strcpy(strings[line++], str);
	}
	fclose(f);
	if (argc == 1) {
		for (int i = 1; i <=  min(line, 10)	; ++i)
			printf("%s", strings[line-i]);
	} else if (argc == 2) {
		len = atoi(argv[1]);
		if (len > min(line, 20)) 
			printf("memory limit exceeded only %d cmds\n", line);
		else {
			printf("%s\n", strings[line -(len-1) - 1]);
		}
	} else {
		printf("history: too many arguments\n");
	}
}

// int main(int argc, char const *argv[]) {
// 	history(argc, argv);
// 	return 0;
// }
// 	// size_t len = 0;
// 	// char *str = NULL;
// 	// open("history.txt", O_RDONLY | O_WRONLY | O_APPEND | O_CREAT, 0644);
// 	// if (errno) {
// 	// 	perror("open");
// 	// 	return 0;
// 	// }
// 	// FILE *f = fopen("history.txt", "r");
// 	// // printf("%d", remove_first());
// 	// for (int i = 0; i < 3; ++i) {
// 	// 	getline(&str, &len, stdin);
// 	// 	if (errno) {
// 	// 		perror("history getline");
// 	// 		return 0;
// 	// 	}
// 	// 	save(str);
// 	// 	// printf("%s", str);
// 	// }
// 	get_info(argc, argv);
// 	return 0;
// }
