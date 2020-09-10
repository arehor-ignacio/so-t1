#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/user.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;
	struct user_regs_struct uregs;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s comando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		if (ptrace(PTRACE_TRACEME, 0 , NULL , NULL)){
			perror("ERROR child ptrace(PTRACE_TRACEME,...)");
			exit(1);
		}
		/* S'olo se ejecuta en el Hijo */
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* S'olo se ejecuta en el Padre */
		while(1) {
			if (wait(&status) < 0) {perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */ 
			int sysno = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);

			if(sysno == 62){
				int a = ptrace(PTRACE_GETREGS, child, NULL, &uregs);
				printf("%lli\n", uregs.rax);
				//int b = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RCX, NULL);
			}
			
			
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);

		}

		
	}
	return 0;
}
