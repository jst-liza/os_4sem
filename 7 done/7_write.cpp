#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

int flag = 0, fd;
pthread_t id_open, id_write;

void sig_handler (int sign) {
	printf ("\n\nget SIGPIPE\n");
}

void* p_write (void* arg){
	char buf [45];	
	while (flag == 0){
		long n_gr_max = sysconf(_SC_NGROUPS_MAX)+1;
		gid_t *gr= (gid_t *)malloc(n_gr_max*sizeof(gid_t));
		int n_gr = getgroups(n_gr_max, gr);
		memset (buf, 0, sizeof buf);
		int len =0;
		for (int i=0; i<n_gr; ++i){
			len += sprintf(&buf[len], "%d ", gr[i]);
		}
		fflush (stdout); 
		printf ("%s\n ", buf);
		ssize_t nw = write(fd, buf, len);
		if (nw == -1) 
			perror("Ошибка записи");
		sleep(2);
		free (gr);
	}
	pthread_exit (NULL);
}

void* p_open (void* arg){
	while (flag == 0){
		fd = open ("/tmp/fifo", O_WRONLY|O_NONBLOCK);
		printf ("\n Открываем именованный канал для передачи");

		if (fd == -1) {
			perror("Ошибка открытия канала");
			sleep(1);
		}

		else {
			pthread_create(&id_write, NULL, p_write, NULL);
			printf ("\n Создан поток для записи");
			pthread_exit (NULL);
		}
	}
}

int main (){
	signal(SIGPIPE, sig_handler);

	printf ("\n Запущена программа передачи");
	
	int ff = mkfifo("/tmp/fifo", 0644);
	printf ("\n Создаем именованный канал");
	if (ff == -1)
		perror("Ошибка создания канала");

	pthread_create(&id_open, NULL, p_open, NULL);
	printf ("\n Создан поток функции открытия");

	printf ("\n Программа ожидает нажатия клавиши");
	getchar();
	printf ("\n Клавиша нажата");
	flag = 1;
	
	pthread_join(id_open, NULL);
	printf ("\n Поток открытия завершен");
	pthread_join(id_write, NULL);
	printf ("\n Поток передачи завершен");

	close(fd);
	printf ("\n Именованный канал закрыт");
	unlink("/tmp/fifo");
	printf ("\n Именованный канал удален");

	printf ("\n Программа передачи завершила работу");
	return 0;
}
