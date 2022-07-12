#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/mman.h>
#include <netdb.h>
#include <fcntl.h>

int flag = 0;
sem_t *sem_w, *sem_r;
int mem_id;
char *addr;

void* stream_2 (void* arg){
	while (flag == 0){
		int value;
		sem_wait(sem_r);
		memcpy(&value, addr, sizeof(int));
		fflush(stdout);
		printf("\n %d  ", value);
		sem_post(sem_w);
	}
	pthread_exit (NULL);
}

int main (){
	pthread_t id;
	printf ("\n Запущена программа 2");

	mem_id = shm_open("/shared", O_CREAT|O_RDWR, 0644);
	printf ("\n Участок разделяемой памяти создан программой 2");
	ftruncate(mem_id, sizeof(_SC_NGROUPS_MAX));
	printf ("\n Размер разделяемой памяти установлен программой 2");
	addr = (char*) mmap(NULL, sizeof(_SC_NGROUPS_MAX), PROT_WRITE|PROT_READ, MAP_SHARED, mem_id, 0);
	printf ("\n Разделяемая память отображена на локальный адрес программой 2");

	sem_r = sem_open ("/sem_w", O_CREAT, 0644, 1);
	sem_w = sem_open ("/sem_r", O_CREAT, 0644, 1);
	printf ("\n Именованный семафоры записи и чтения открыты(созданы) программой 2");

	pthread_create(&id, NULL, stream_2, NULL);
	printf ("\n Поток создан программой 2");

	printf ("\n Программа 2 ожидает нажатия клавиши");
	getchar();
	flag = 1;
	printf ("\n Клавиша нажата");

    	pthread_join(id, NULL);
    	printf ("\n Поток закрыт программой 2");

	sem_close(sem_w);
    	sem_close (sem_r);
    	printf ("\n Семафоры закрыты программой 2");

    	sem_unlink("/sem_w");
    	sem_unlink("/sem_r");
    	printf ("\n Семафоры удалёны программой 2");

    	munmap(addr, NI_MAXHOST);
    	printf ("\n Отображение разделяемой памяти удалено программой 2");
    	close(mem_id);
    	printf ("\n Участок разделяемой памяти закрыт программой 2");
    	shm_unlink("/shared");
    	printf ("\n Участок разделяемой памяти удален программой 2");

    	printf ("\n Работа программы 2 завершена");

	return 0;
}
