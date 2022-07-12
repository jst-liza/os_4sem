#include <iostream>
#include <unistd.h>
#include <cstdio>

int main (int argsc, char** args){
	printf ("\n Запущен дочерний процесс");
	printf ("\n Идентификатор процесса: ");
	printf ("%d", getpid());
	printf ("\n Идентификатор родительского процесса: ");
	printf ("%d", getppid());
	printf ("\n Принятые аргументы:\n");
	for (int i = 1; i < 5; ++i){
		printf ("%s\n", args[i]);
		fflush (stdout);
		sleep(1);
	}
	exit(15);
}
