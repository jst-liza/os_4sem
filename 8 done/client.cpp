#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <vector>
#include <signal.h>
#define SERVER "serv_sock"
#define CLIENT "cl_sock"

int client_socket, flag_answer = 0, flag_send_request = 0;
sockaddr_un client_socket_addr, server_socket_addr; 
pthread_t id_send_request, id_answer;
pthread_mutex_t mut;
socklen_t addr_len;

void sig_handler (int sign){
	printf ("\n\nget SIGPIPE\n");
}


void* send_request (void* arg){	
	int count=0;
	while (flag_send_request==0){
		char sndbuf [128];
		
		int len = sprintf(sndbuf, "request%d", count);
		int sentcount = sendto(client_socket, sndbuf, len, 0, (struct sockaddr*)&server_socket_addr, sizeof(server_socket_addr));
		if (sentcount == -1){
			perror("send");
		}
		else {
			fflush (stdout);
			printf ("\nОтправлен запрос номер %d", count);
			count++;
		}
		sleep(1);
	}
	pthread_exit (NULL);
}


void* answer (void* arg){
	char rcvbuf [128];
	int count=0;	
	while (flag_answer ==0){
		memset(rcvbuf, 0, sizeof(rcvbuf));
		int reccount = recvfrom(client_socket, rcvbuf, sizeof(rcvbuf), 0, (struct sockaddr*)&server_socket_addr, &addr_len);
		if (reccount == -1){
			 perror ("recv");
			 sleep(1);
		}
		else if (reccount==0){
			sleep (1);
		}
		else {
			fflush (stdout);	
			printf ("\nПолучен ответ %d: \n%s\n", count, rcvbuf);
			count++;
		}
	}
	pthread_exit (NULL);
}


int main (){
	signal(SIGPIPE, sig_handler);
	
	printf ("\n Запущен клиент");
	
	client_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
	fcntl(client_socket, F_SETFL, O_NONBLOCK);
	client_socket_addr.sun_family = AF_UNIX;
	strcpy(client_socket_addr.sun_path, CLIENT);
	unlink(CLIENT);
	int len = sizeof (client_socket_addr.sun_family) + strlen(client_socket_addr.sun_path);
	if (bind(client_socket, (struct sockaddr*)&client_socket_addr, len) < 0)
		perror("bind");
	int optval =1; 
	setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));
	printf ("\n Создан и привязан сокет клиента");

	server_socket_addr.sun_family = AF_UNIX;
	strcpy(server_socket_addr.sun_path, SERVER);
	//printf ("\n Привязан сокет сервера");

	pthread_create(&id_send_request, NULL, send_request, NULL);
	printf ("\n Запущен поток отправки запросов");
	pthread_create(&id_answer, NULL, answer, NULL);
	printf ("\n Запущен поток получения ответов");
	
	printf ("\n Программа ожидает нажатия клавиши");
	getchar();
	printf ("\n Клавиша нажата");
	flag_answer = 1;
	flag_send_request = 1;
	
	pthread_join (id_send_request, NULL);
	printf ("\n Закрыт поток отправки запросов");
	pthread_join (id_answer, NULL);
	printf ("\n Закрыт поток получения ответов");
	
	//shutdown (client_socket, 2); 
	close (client_socket);
	printf ("\n Сокет клиента закрыт");
	
	printf ("\n Клиент выключен");
	return 0; 
}
