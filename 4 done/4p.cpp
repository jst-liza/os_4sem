#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char **argv){

    	printf ("\n Программа запущена");
	printf ("\n Запущен родительский процесс");
	printf ("\n Идентификатор процесса: ");
	printf ("%d", getpid());
	printf ("\n Идентификатор родительского процесса: ");
	printf ("%d", getppid());

    	printf ("\n Введите четыре аргумента: \n");
	for (int i = 1; i < 5; ++i){
		argv[i]=(char*)malloc(20*sizeof(char));
		scanf ("%19s", argv[i]);
	}

	pid_t pid = fork();
	
	if (pid == 0){
		execvp("./4d", NULL);
	}
	else if (pid == -1){
		printf ("\n Ошибка создания дочернего процесса");
	}
	else {
		printf ("\n Идентификатор дочернего процесса: ");
		printf ("%d", pid);
		int N; 
		while (waitpid(pid, &N, WNOHANG)==0){
			printf ("\n Ждем...");
			sleep(1.5);
		}
		printf ("\n Код завершения дочернего процесса: ");
        	printf ("%d", WEXITSTATUS(N));
	}
	
    	printf ("\n Программа завершила работу\n");
	return 0;
}
