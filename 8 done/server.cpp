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

int server_socket, flag_answer=0, flag_get_request=0;
pthread_t id_get_request,id_answer;
pthread_mutex_t mut;
std::vector <std::string> msglist; 
sockaddr_un server_socket_addr, client_socket_addr;
socklen_t addr_len;


void sig_handler (int sign){
	printf ("\n\nget SIGPIPE\n");
}


void* get_request (void* arg){	
	char rcvbuf[128];
	int count=0;
	while (flag_get_request==0){
		int reccount = recvfrom(server_socket, rcvbuf, sizeof(rcvbuf), 0, (struct sockaddr*)&client_socket_addr, &addr_len);
		if (reccount == -1){
			perror ("recv");
			sleep(1);
		}
		else if (reccount == 0) {
			sleep(1);
		}
		else {
			pthread_mutex_lock(&mut);
			msglist.push_back(std::string(rcvbuf));
			pthread_mutex_unlock(&mut);
			fflush (stdout);
			printf ("\nПолучен запрос номер %d", count);
			count++;
		}
	}
	pthread_exit (NULL);
}


void* answer (void* arg){
	int count=0;
	while (flag_answer == 0){
		pthread_mutex_lock(&mut);
		if (!msglist.empty()){
			pthread_mutex_unlock(&mut);
			std::string s = msglist.back();
			msglist.pop_back();
			
			char buf [45];	
			long n_gr_max = sysconf(_SC_NGROUPS_MAX)+1;
			gid_t *gr= (gid_t *)malloc(n_gr_max*sizeof(gid_t));
			int n_gr = getgroups(n_gr_max, gr);
			memset (buf, 0, sizeof buf);
			int len =0;
			for (int i=0; i<n_gr; ++i){
			len += sprintf(&buf[len], "%d ", gr[i]);
			}
			
			int sentcount = sendto(server_socket, buf, len, 0, (struct sockaddr*)&client_socket_addr, sizeof(client_socket_addr));
			if (sentcount==-1) 
				perror ("send");
			else {
				fflush (stdout);
				printf("\nСообщение %d оптравлено!\n", count);
				count++; 
			}
		} else {
			pthread_mutex_unlock(&mut);
			sleep(1);
		}
	}
	pthread_exit (NULL);
}


int main (){
	signal(SIGPIPE, sig_handler);
	pthread_mutex_init(&mut, NULL);
	
	printf ("\n Запущен сервер");
	
	server_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
	fcntl (server_socket, F_SETFL, O_NONBLOCK);
	server_socket_addr.sun_family = AF_UNIX;
	strcpy(server_socket_addr.sun_path, SERVER);
	unlink (SERVER);
	int len = sizeof (server_socket_addr.sun_family) + strlen(server_socket_addr.sun_path);
	if (bind(server_socket, (struct sockaddr*)&server_socket_addr, len) < 0)
		perror("bind");
	int optval =1; 
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));
	printf ("\n Создан и привязан сокет сервера");
	
	client_socket_addr.sun_family = AF_UNIX;
	strcpy(client_socket_addr.sun_path, CLIENT);
	//printf ("\n Привязан сокет клиента");
	
	pthread_create(&id_get_request, NULL, get_request, NULL);
	printf ("\n Запущен поток получения запросов");
	pthread_create(&id_answer, NULL, answer, NULL);
	printf ("\n Запущен поток ответа на запросы");
	
	printf ("\n Программа ожидает нажатия клавиши");
	getchar();
	printf ("\n Клавиша нажата");
	flag_answer = 1;
	flag_get_request = 1;
	
	pthread_join(id_get_request, NULL);
	printf ("\n Закрыт поток получения запросов");
	pthread_join(id_answer, NULL);
	printf ("\n Закрыт поток ответа на запросы");
	
	//shutdown(server_socket, 2); 
	close(server_socket);
	printf ("\n Сокет сервера закрыт");
	
	printf ("\n Сервер выключен");
	return 0;
}

