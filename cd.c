#include "headers.h"
#include "macros.h"
#include "modify_path.h"

char prev[2][50] = {".", "."};
void cd(int argc, char *begin, char **argv) {
	char cwd[50];
	// for (int i = 0; i < argc; ++i)
 //        printf("%s/", argv[i]);
	if (errno)
		perror("getpwd in cd");
	if (argc > 2) {
		printf("cd: too many arguments\n");
		return;
	} else {
		if (argc == 1 || !strcmp(argv[1], "~"))
			chdir(begin);
		else if (!strcmp(argv[1], "-")) {
			chdir(prev[0]);
			strcpy(cwd, prev[0]);
			modify_path(begin, cwd);
			printf("%s\n", cwd);
		}
		else
			chdir(argv[1]);
		
		if (errno)
			perror("chdir");
		getcwd(cwd, SIZE);
		strcpy(prev[0], prev[1]);
		strcpy(prev[1], cwd);
	}
}

// int main(int argc, char const *argv[]) {
// 	char *args[] = {"cd"};
// 	cd(2, args);
// 	return 0;
// }