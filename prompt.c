#include "headers.h"
#include "macros.h"
#include "prompt.h"
#include "modify_path.h"


void prompt(char *begin) {
	char username[100];
	char hostname[100];
	char str[100];
	char cwd[100];

	struct passwd *pass; pass = getpwuid(getuid());
	// printf("%d %s\n", getlogin_r(username, 99), username);
	char* name = pass->pw_name;
	// printf("%s", name);
	// if (getlogin_r(username, 99) != 0) {
	// 	// perror("username");
	// 	return;
	// }
	if (gethostname(hostname, 99) != 0) {
		perror("hostname");
		return;
	}
	if (getcwd(cwd, SIZE) == NULL) {
		perror("working directory");
		return;
	}

	strcpy(str, "<");
	strcat(str, name);
	strcat(str, "@");
	strcat(str,hostname);
	strcat(str, ":");
	modify_path(begin, cwd);
	strcat(str, cwd);
	strcat(str, ">");
	printf("%s", str);
}
