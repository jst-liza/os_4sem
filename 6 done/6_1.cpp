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

int flag = 1;
sem_t *sem_w, *sem_r;
int mem_id;
char *addr; 

void* stream_1 (void* arg){
	while (flag == 1){
		//Функция из 4 лабы
		long n_gr_max = sysconf(_SC_NGROUPS_MAX)+1;
		gid_t *groups= (gid_t *)malloc(n_gr_max*sizeof(gid_t));
		int n_gr = getgroups(n_gr_max, groups);

		printf ("\n%d ", n_gr);
		fflush (stdout);
		
		memcpy(addr, &n_gr, sizeof(int));
		sem_post(sem_r);
		sem_wait(sem_w);
        free(groups);
		sleep(1);
	}
	pthread_exit (NULL);
}

int main (){
	pthread_t id;
    	printf ("\n Запущена программа 1");

    	mem_id = shm_open("/shared", O_CREAT|O_RDWR, 0644);
    	printf ("\n Участок разделяемой памяти создан программой 1");
    	ftruncate(mem_id, sizeof(_SC_NGROUPS_MAX));
    	printf ("\n Размер разделяемой памяти установлен программой 1");
    	addr = (char*) mmap(NULL, sizeof(_SC_NGROUPS_MAX), PROT_WRITE|PROT_READ, MAP_SHARED, mem_id, 0);
    	printf ("\n Разделяемая память отображена на локальный адрес программой 1");

	sem_r = sem_open ("/sem_w", O_CREAT, 0644, 1);
	sem_w = sem_open ("/sem_r", O_CREAT, 0644, 1);
    	printf ("\n Именованный семафоры записи и чтения открыты(созданы) программой 1");
	
    	pthread_create(&id, NULL, stream_1, NULL);
    	printf ("\n Поток создан программой 1");

    	printf ("\n Программа 1 ожидает нажатия клавиши");
	getchar();
	flag = 2;
	printf ("\n Клавиша нажата");

    	pthread_join(id, NULL);
    	printf ("\n Поток закрыт программой 1");

	sem_close(sem_w);
   	sem_close (sem_r);
    	printf ("\n Семафоры закрыты программой 1");

    	sem_unlink("/sem_w");
    	sem_unlink("/sem_r");
    	printf ("\n Семафоры удалёны программой 1");

    	munmap(addr, NI_MAXHOST);
    	printf ("\n Отображение разделяемой памяти удалено программой 1");
    	close(mem_id);
    	printf ("\n Участок разделяемой памяти закрыт программой 1");
    	shm_unlink("/shared");
    	printf ("\n Участок разделяемой памяти удален программой 1");

    	printf ("\n Работа программы 1 завершена");
    	
    	return 0;
}
