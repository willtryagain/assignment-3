#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main (int argc, char *argv[]) {

	DIR *dp;
	struct dirent *dirp;

	if(argc ==1) 
		dp = opendir ("./");
	else 
		dp = opendir(argv[1]);
	if (dp == NULL)
		printf("null");
	while ((dirp = readdir(dp)) != NULL) 
		printf("%s\n", dirp->d_name);

	closedir(dp);
	exit(0);
}