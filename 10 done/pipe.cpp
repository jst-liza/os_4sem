 #include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <ev.h>

int flag_1 = 0, flag_2 = 0, done = 0;
ev_io enter_watcher, input_watcher;
ev_timer timeout_watcher;
int pipefd[2];
char buf [32];

static void enter_cb(EV_P_  ev_io *w, int revents) {
	printf (" Клавиша нажата");
	ev_io_stop(EV_A_  w);
	ev_break(EV_A_  EVBREAK_ALL);
}

static void write_cb(EV_P_ ev_timer *w, int revents) {
	long n_gr_max = sysconf(_SC_NGROUPS_MAX)+1;
	gid_t *groups= (gid_t *)malloc(n_gr_max*sizeof(gid_t));
	int n_gr = getgroups(n_gr_max, groups);
	memset(buf, 0, sizeof buf);
	int len = 0;
	for (int i = 0; i < n_gr; ++i){
		len += sprintf(&buf[len], "%d ", groups[i]);
	}
	ssize_t nw = write(pipefd[1], buf, len);
	if (nw == -1)
            	perror("write");
	free (groups);
}

static void read_cb(EV_P_ ev_io *w, int revents) {
	memset (buf, 0, sizeof buf);
	ssize_t nr= read(pipefd[0], buf, sizeof buf);
	if (nr == -1)
		perror("read");
	printf ("%s\n", buf);
	ev_timer_again (loop, &timeout_watcher);
}



int main (){
	struct ev_loop *loop = EV_DEFAULT;
	pipe(pipefd);
	
	printf ("\n Программа запущена");
		
	ev_timer_init(&timeout_watcher, write_cb, 1., 1.);
	ev_io_init(&input_watcher, read_cb, pipefd[0], EV_READ);
	ev_io_init(&enter_watcher, enter_cb, STDIN_FILENO, EV_READ);
	printf ("\n Инициализированы обработчики событий на ввод(таймер), чтение(pipe) и нажатие клавиши(STDIN)");
	
	ev_timer_start(loop, &timeout_watcher);
	ev_io_start(loop, &input_watcher);
	ev_io_start(loop,  &enter_watcher);
	printf ("\n Обработчики событий на ввод(таймер), чтение(pipe) и нажатие клавиши(STDIN) привязаны к циклу ожидания обработки событий");
	
	printf ("\n Запущен цикл ожидания и обработки событий(программа ожидает нажатия клавиши)\n");
	ev_run(EV_DEFAULT_  0);
	ev_timer_again (loop, &timeout_watcher);
	
	close(pipefd[0]);
	close(pipefd[1]);
	printf ("\n Pipe закрыт(ввод и чтение)");

	printf ("\n Программа завершена\n");
	return 0;
}
