#include "headers.h"
#include "modify_path.h"
#include "macros.h"


int parse_line(char *line) {
	//for kb
	int n = strlen(line);
	const char* p = line;
	while (*p < '0' || *p > '9')
		*p++;
	line[n-3] = '\0';
	return atoi(p);
}

void get_vmsize_state(char *path, int *VmSize, char *State) {
	FILE *file = NULL;
	char line[200]; 

	if ((file = fopen(path, "r")) == NULL) {
      // perror("pinfo fopen");
      return;
    }
    
    while (fgets(line, 128, file) != NULL) {
    	if (strncmp(line, "VmSize", 6) == 0) 
    		*VmSize = parse_line(line);
    	else if (strncmp(line, "State", 5) == 0) 
    		*State = line[7];
    }
}

void get_ex_path(char *path, char *ex_path) {
	readlink(path, ex_path, SIZE);
	if (errno) 
      perror("executable path");
}

void pinfo(int argc, char** argv) {
	// for (int i = 0; i < argc; ++i)
	// 	printf("%s/", argv[i]);
	int VmSize = -1;
	char path_v[100] = "/proc/";
	char path_e[SIZE] = "/proc/";
	char State = '\0';
	char ex_path[SIZE] = "NULL";

	// for (int i = 0; i < argc; ++i) {
	// 	if (argv[i] == NULL) {
	// 		printf("%d/", i);
	// 	} else
	// 	printf("%s/", argv[i]);
	// }
	if (argc == 2) {
		// printf("%s\n", argv[1]);
		strcat(path_v, argv[1]);
		strcat(path_e, argv[1]);
	} else if (argc == 1) {
		strcat(path_v, "self");
		strcat(path_e, "self");
	} else {
		printf("pinfo: too many arguments\n");
		return;
	}
	strcat(path_v, "/status");
	strcat(path_e, "/exe");
	get_vmsize_state(path_v, &VmSize, &State);
	get_ex_path(path_e, ex_path);
	// printf("\n");
	if (argc == 2) 
		printf("pid -- %s\n", argv[1]);
	else
		printf("pid -- %d\n", getpid());
	printf("Process Status -- %c\n", State);
	printf("memory -- %d\n", VmSize);
	modify_path("/home/name/Documents/operating", ex_path);
	printf("Executable Path -- %s\n", ex_path);
}

