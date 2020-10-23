#include "headers.h"
#include "macros.h"
#include "prompt.h"
#include "modify_path.h"


void prompt(char *begin) {
	char username[100];
	char hostname[100];
	char str[100];
	char cwd[100];
	
	
	struct passwd *pws;
	pws = getpwuid(getuid());
	strcpy(username, pws->pw_name);
	/*printf("%d %s\n", getlogin_r(username, 99), username);
	if (getlogin_r(username, 99) != 0) {
		perror("username");
		return;
	}*/
	if (gethostname(hostname, 99) != 0) {
		perror("hostname");
		return;
	}
	if (getcwd(cwd, SIZE) == NULL) {
		perror("working directory");
		return;
	}

	strcpy(str, "<");
	strcat(str, username);
	strcat(str, "@");
	strcat(str,hostname);
	strcat(str, ":");
	modify_path(begin, cwd);
	strcat(str, cwd);
	strcat(str, ">");
	printf("%s", str);
}
