#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int flag_1 = 0, flag_2 = 0;
sem_t sem;

void* stream_1 (void* arg){
	while (flag_1 == 0){
		timespec t;
		clock_gettime(CLOCK_REALTIME, &t);
		t.tv_sec += 1;
		int rv = sem_timedwait(&sem, &t);
		
		while (rv != 0){
            perror("sem_timedwait");
            if (flag_1 == 1)
                pthread_exit (NULL);
            clock_gettime(CLOCK_REALTIME, &t);
            t.tv_sec += 1;
            rv = sem_timedwait(&sem, &t);
		}
		
		for (int i = 0; i < 5; ++i){
			printf ("%d", 1);
			fflush (stdout);
			sleep(1);
		}
		sem_post(&sem);
		sleep(1);
	}
	pthread_exit (NULL);
}

void* stream_2 (void* arg){
	while (flag_2 == 0){
		timespec t;
		clock_gettime(CLOCK_REALTIME, &t);
		t.tv_sec += 1;
		int rv = sem_timedwait(&sem, &t);
		
		while (rv!=0){ 
            perror("sem_timedwait");
            if (flag_1 == 2)
                pthread_exit (NULL);
            clock_gettime(CLOCK_REALTIME, &t);
            t.tv_sec += 1;
            rv = sem_timedwait(&sem, &t);
		}
		
		for (int i = 0; i < 4; ++i){
			printf ("%d", 2);
			fflush (stdout);
			sleep(1);
		}
		sem_post(&sem);
		sleep(1);
	}
	pthread_exit (NULL);
}

int main (){
	pthread_t id1, id2;
	sem_init (&sem, 0, 1);

    printf ("\n Программа запущена");

	pthread_create(&id1, NULL, stream_1, NULL);
	printf ("\n Поток 1 начал работу\n");

	pthread_create(&id2, NULL, stream_2, NULL);
	printf ("\n Поток 2 начал работу\n");

	printf ("\n Программа ожидает нажатия клавиши\n");
	getchar();
	printf (" Клавиша нажата");

	flag_1 = 1;
	flag_2 = 1;

    pthread_join(id1, NULL);
	printf ("\n Поток 1 закончил работу");
	pthread_join(id2, NULL);
	printf ("\n Поток 2 закончил работу");
	sem_destroy (&sem);

    printf ("\n Программа завершила работу\n");
	return 0;
}
