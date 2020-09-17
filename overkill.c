#include "headers.h"

void overkill() {
	extern pid_t bg_pids[50];
	extern int total;
	for (int i = 0; i < total; ++i) {
		if (kill(bg_pids[i], SIGKILL) < 0)
			perror("overkill"); 
	}
}