#include "headers.h"
#include "jobs.h"

extern char process_name[50][SIZE];
extern pid_t bg_pids[50];
extern int total;


void get_state(char *path, char *State) {
	FILE *file = fopen(path, "r");
	if (errno) {
      perror("jobs fopen");
      return;
    }
    char line[SIZE];
    while (fgets(line, 128, file) != NULL) {
    	if (strncmp(line, "State", 5) == 0) {
    		*State = line[7];
    		break;
    	}
    }
}


void jobs(int argc, char argv[][SIZE]) {
	char path[100]; 
	char State = '\0';
	for (int i = 0; i < total; ++i) {
		char id[12];
		strcpy(path, "/proc/");
		sprintf(id, "%d", bg_pids[i]);
		strcat(path, id);
		strcat(path, "/status");
		get_state(path, &State);
		printf("[%d]", i+1);

		if (State == 'R')
			printf(" running");
		else
			printf(" stopped");
		printf(" %s [%d]\n", process_name[i], bg_pids[i]);
	}
}