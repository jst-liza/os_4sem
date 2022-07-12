#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

	printf ("\n Запущена программа");
	
	printf("\n Текущий пользователь = \'%s\'", getenv("USER"));
	
	if (int fd = open("test.txt", O_RDWR) == -1)
		perror("open");
	else{ 
		printf("\n Файл успешно открыт");
		close(fd);
	}

	printf ("\n Программа завершила свою работу");
	return 0;
}
