#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int handler_calls = 0;
int status = 0;

void SIGURG_handler(int sig) {
	write(1, "ya va!\n",7);
	handler_calls++;
}

void SIGINT_handler(int sig) {
	while (1) {
		if (wait(&status) < 0) perror("wait");
		if(WIFEXITED(status)) break;
	}
	exit(1);
}

void child(char* path, char* argv[]) {
	signal(SIGURG,SIGURG_handler);
	while (handler_calls < 4) {}
	pid_t p_pid = getppid();
	kill(p_pid, SIGINT);
	execvp(path, argv);
	exit(1);
}

void parent(pid_t h_pid) {
	signal(SIGINT, SIGINT_handler);
	while (1) {
		write(1, "Dale Loco!\n",11);
		kill(h_pid, SIGURG);
		sleep(1);
	}
}

int main(int argc, char* argv[]) {

	pid_t h_pid = fork();
	if (!h_pid) child(argv[1], argv +1);
	else parent(h_pid);

	return 0;
}