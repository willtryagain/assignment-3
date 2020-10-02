#include "headers.h"
#include "macros.h"
#include "modify_path.h"

char prev[2][50] = {"~", "~"};
void cd(int argc, char *begin, char **argv) {
	char cwd[50];
	if (!strcmp(prev[0], "~"))
		strcpy(prev[0], begin);
	if (!strcmp(prev[1], "~"))
		strcpy(prev[1], begin);
	strcpy(prev[0], prev[1]);
	getcwd(prev[1], 50);
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
			fprintf(stderr, "%s\n", cwd);
		}
		else {
			if (chdir(argv[1]) < 0)
				perror("chd");
		}
		
	}
}

// int main(int argc, char const *argv[]) {
// 	char *args[] = {"cd"};
// 	cd(2, args);
// 	return 0;
// }