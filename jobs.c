#include "headers.h"
#include "pinfo.h"


extern int total;
extern char process_name[50][25];
extern pid_t bg_pids[50];

void jobs(int argc, char argv[][500]) {
	int VmSize;
	char State = '\0';
	for (int i = 0; i < total; ++i) {
		char path[100] = "/proc/";
		char id[12];
		char action[25];
		sprintf(id, "%d", bg_pids[i]);
		strcat(path, id);
		strcat(path, "/status");
		get_vmsize_state(path, &VmSize, &State);
		// printf("%s\n", );
		switch (State) {
			case 'R':
			case 'S':
				strcpy(action, " running");
				break;
			case 'T':
				strcpy(action, " stopped");
				break;

			default:
				strcpy(action, "stopped");
		}
		printf("[%d] %s %s [%d]\n", i+1, action, process_name[i], bg_pids[i]);
	}
	// for (int i = 0; i < total; ++i)
	// 	fprintf(stderr, "%s/", process_name[i]);
}