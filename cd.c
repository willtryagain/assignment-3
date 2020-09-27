#include "headers.h"
#include "macros.h"


void cd(int argc, char *begin, char **argv) {
	char cwd[SIZE];
	// for (int i = 0; i < argc; ++i)
 //        printf("%s/", argv[i]);
	getcwd(cwd, SIZE);
	if (errno)
		perror("getpwd in cd");
	if (argc > 2) {
		printf("cd: too many arguments\n");
		return;
	} else {
		if (argc == 1 || !strcmp(argv[1], "~"))
			chdir(begin);
		else{
			chdir(argv[1]);
		}
		if (errno)
			perror("chdir");
		
	}
}

// int main(int argc, char const *argv[]) {
// 	char *args[] = {"cd"};
// 	cd(2, args);
// 	return 0;
// }