#include <pthread.h>
#include <stdio.h>

struct char_print_params {
	char character;
	int count;
};

void* char_print(void* parameters) {
	struct char_print_params *p = (struct char_print_params *) parameters;

	int i;
	for (i = 0; i < p->count; i++)
		fputc(p->character, stderr);
	return NULL;
}

int main() {
	pthread_t thread_id1;
	pthread_t thread_id2;
	struct char_print_params thread1_args;
	struct char_print_params thread2_args;

	thread1_args.character = 'x';
	thread1_args.count = 300;
	pthread_create(&thread_id1, NULL, &char_print, &thread1_args);

	thread2_args.character = 'o';
	thread2_args.count = 500;
	pthread_create(&thread_id2, NULL, &char_print, &thread2_args);

	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);

	return 0;
}
