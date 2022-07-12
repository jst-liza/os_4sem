#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <stdlib.h>

void* stream_1 (void* arg) {
	int *flag = (int*) arg; 
	while (*flag == 0){
	    printf ("%d", 1);
	    fflush (stdout);
	    sleep(1);
	}
	pthread_exit ((void*)0);
}

void* stream_2(void* arg) {
	int *flag = (int*) arg; 
	while (*flag == 0){
	    printf ("%d", 2);
	    fflush (stdout);
	    sleep(1);
	}
	pthread_exit ((void*)0);
}



int main () {
	int flag_1 = 0, flag_2 = 0;
	int attr_1, attr_2;
	int status_1, status_2;
	pthread_t id1, id2;
	pthread_attr_t aid1, aid2;
	
	printf ("\n Программа запущена");
	
	pthread_create(&id1, NULL, stream_1, &flag_1);
	printf ("\n Поток 1 начал работу\n");

	
	pthread_create(&id2, NULL, stream_2, &flag_2);
	printf ("\n Поток 2 начал работу\n");

	printf ("\n Программа ожидает нажатия клавиши\n");
	getchar();
	printf (" Клавиша нажата");	
		
	flag_1 = 1;
	flag_2 = 1;
	
	pthread_join(id1, (void**) &status_1);
	printf ("\n Поток 1 закончил работу");
	pthread_join(id2, (void**) &status_2);
	printf ("\n Поток 2 закончил работу");
	printf ("\n Код завершения потока 1: ");
	printf ("%d", status_1);
	printf ("\n Код завершения потока 2: ");
	printf ("%d", status_2);

	printf ("\n Программа завершила работу\n");
	return 0;
}

