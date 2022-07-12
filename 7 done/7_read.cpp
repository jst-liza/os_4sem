#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int flag = 0, fd;

void* p (void* arg){	
	char buf [45];
	memset (buf, 0, sizeof buf);
	while (flag == 0){
		ssize_t nr = read(fd, buf, sizeof buf);
		if (nr == -1) 
			perror("Ошибка чтения");
		else {
			fflush (stdout); 
			printf ("%s\n", buf);
		}
		sleep(2);
	}
	pthread_exit (NULL);
}

int main (){
	pthread_t id;
	
	printf ("\n Запущена программа чтения");
	
	int ff = mkfifo("/tmp/fifo", 0644);
	printf ("\n Создаем именованный канал");
	if (ff == -1) 
		perror("Ошибка создания канала");
	
	fd = open ("/tmp/fifo", O_RDONLY|O_NONBLOCK);
	printf ("\n Открываем именованный канал для чтения");
	if (fd == -1) 
		perror("Ошибка открытия канала");
	
	pthread_create(&id, NULL, p, NULL);
	printf ("\n Создан поток для чтения");

	printf ("\n Программа ожидает нажатия клавиши");
	getchar();
	printf ("\n Клавиша нажата");
	flag = 1;
	
	pthread_join(id, NULL);
	printf ("\n Поток для чтения завершен");
	
	close(fd);
	printf ("\n Именованный канал закрыт");
	unlink("/tmp/fifo");
	printf ("\n Именованный канал удален");

	printf ("\n Программа передачи завершила работу");
	return 0;
}
