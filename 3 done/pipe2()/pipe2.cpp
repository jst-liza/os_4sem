#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>

int flag_1 = 0, flag_2 = 0;
int pipefd[2];

void* stream_1 (void* arg){
	char buf [32];
	while (flag_1 == 0){
		long n_gr_max = sysconf(_SC_NGROUPS_MAX)+1;
		gid_t *groups= (gid_t *)malloc(n_gr_max*sizeof(gid_t));
		int n_gr = getgroups(n_gr_max, groups);
		memset(buf, 0, sizeof buf);
		int len = 0;
		for (int i = 0; i < n_gr; ++i){
		    len += sprintf(&buf[len], "%d", groups[i]);
		}
		ssize_t nw = write(pipefd[1], buf, len);
		if (nw == -1)
            perror("write");
		sleep(1);
		free (groups);
		}
	pthread_exit (NULL);
}

void* stream_2 (void* arg){
	char buf [32];
	while (flag_2 == 0){
		memset (buf, 0, sizeof buf);
		ssize_t nr= read(pipefd[0], buf, sizeof buf);
		if (nr == -1)
            		perror("read");
		printf ("%s\n", buf);
		sleep(1);
		}
	pthread_exit (NULL);
}

int main (){
	pthread_t id1, id2;

    printf ("\n Программа запущена");

    pipe(pipefd);
    printf ("\n Неименованный канал создан");
    int flag = 0;
	flag |= O_NONBLOCK;
	pipe2(pipefd, flag);
    printf ("\n Дескрипторам установлено неблокируемое состояние");

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

	close(pipefd[0]);
    printf ("\n Неименованный канал(чтение) закрыт");
	close(pipefd[1]);
    printf ("\n Неименованный канал(запись) закрыт");

    printf ("\n Программа завершила работу\n");
	return 0;
}
