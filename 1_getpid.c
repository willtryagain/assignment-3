#include "headers.h"

void set_dir(char *path) {
	char cwd[SIZE];
	chdir(path);
	chdir("..");
	getcwd(global_dir, SIZE);
}

int main() {
	int pid = getpid();
	printf("My process ID is %d\n", pid);
	// for (int i = 0; i < 3; ++i)
	// 	printf("%d\n", getpid());
	// for (;;);
	char cwd[1024];
	char *argv = "willtryagain";
	chdir(argv);
	chdir("..");
	getcwd(cwd, 1024);
	printf("%s", cwd);
	return 0;
}