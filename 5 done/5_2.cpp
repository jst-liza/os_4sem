#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int flag = 0;
sem_t* sem;
FILE* test;

void* stream (void* arg){
	while (flag == 0){
		sem_wait(sem);
		for (int i = 0; i < 4; ++i){
			printf("%d", 2);
			fflush(stdout);
			fprintf(test, "%d", 2);
			fflush(test);
			sleep(1);
		}
		sem_post(sem);
		sleep(2);
	}
	pthread_exit (NULL);
}

int main (){
	pthread_t id;
    printf ("\n Запущена программа 2");
	sem = sem_open("/semname", O_CREAT, 0644, 1);
    printf ("\n Именованный семафор открыт(создан) программой 2");
	test = fopen("lab5.txt", "a+");
    printf ("\n Файл открыт программой 2");
	pthread_create(&id, NULL, stream, NULL);
    printf ("\n Поток создан программой 2");
    printf ("\n Программа 2 ожидает нажатия клавиши");
	getchar();
	flag = 1;
	printf ("\n Клавиша нажата");
	pthread_join(id, NULL);
    printf ("\n Поток закрыт программой 2");
	sem_close(sem);
    printf ("\n Семафор закрыт программой 2");
	sem_unlink("/semname");
    printf ("\n Семафор удалён программой 2");
    printf ("\n Работа программы 2 завершена");

	return 0;
}