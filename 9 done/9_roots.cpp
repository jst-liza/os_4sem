#include <sys/capability.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define new_caps "CAP_DAC_OVERRIDE=+eip"


int main(void) {
	
	cap_t caps_old, caps_new;
	char *caps_text;
	ssize_t length;
	
	printf ("\n Запущена программа проверки и установки POSIX-возможностей");
	
	caps_old = cap_get_file("9_test");
	if (caps_old == NULL) {
		perror("cap_get_file");
		exit(-1);
	}
	
	caps_text = cap_to_text(caps_old, &length);
	printf("\n Перечень POSIX-возможностей");
	printf("%s\n", caps_text);

	printf("\n Установить новые значения POSIX-возможностей? Y/N\n");
	if (getchar() == 'Y') {
		caps_new = cap_from_text(new_caps);
		if (cap_set_file("9_test", caps_new) == 0)
			perror("cap_set_file");
		else
			printf("\n Успешно установлены");
	}
	
	cap_free(&caps_text);
	
	printf ("\n Программа завершила свою работу");
	return 0;
}
