// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>

#define MAX_PHYLOS 15
#define MIN_PHYLOS 5

#define LEFT     (i + current - 1) % current
#define RIGHT    (i + 1) % current
#define THINKING 0
#define HUNGRY   1
#define EATING   2

int philo(char ** num);
void addPhylo();
void remove();
void think();
void forks(int i);
void test(int i);
void eat();
void put(int i);

int pids[MAX_PHYLOS] = {0};

int status[MAX_PHYLOS] = {0};
int s[MAX_PHYLOS] = {0};
int alt[MAX_PHYLOS] = {0};

int8_t mutex;
int8_t printID;

int last;
int current;

int phylo(char ** arguments) {
	printChar('\n');
	printf("Welcome to the dining philosophers.\n");
	printf("Press A to add a philosopher.\n");
	printf("Press R to remove a philosopher.\n");
	printf("Press Q to quit.\n");

	last = 0;
	current = 0;
	mutex = sys_sem_create("mutex", 1);
	printID = sys_sem_create("print", 1);
	sys_sem_open("mutex");
	sys_sem_open("print");

	printf("Initializing...\n");
	sys_wait(10);
	for (int i = 0; i < MIN_PHYLOS; i++) {
		addPhylo();
	}

	char c;
	while (!last) {
		c = getChar();

		if (c == 'A' || c == 'a') {
			printf("A new philosofer got hungry hmmmm...\n");
			sys_wait(5);
			addPhylo();
		}
		if (c == 'R' || c == 'r') {
			printf("Philosopher is no longer hungry...\n");
			sys_wait(5);
			remove();
		}
		if (c == 'Q' || c == 'q') {
			printf("That's all folks\n");
			last = 1;
		}
	}
	// sys_wait_pid(pids[0]);
	for (int i = 0; i < current; i++) {
		sys_kill_process(pids[i]);
		sys_kill_process(pids[i]);
		sys_sem_close(s[i]);
		sys_sem_close(alt[i]);
		sys_sem_delete(s[i]);
		sys_sem_delete(alt[i]);
	}
	sys_sem_close(mutex);
	sys_sem_close(printID);
	sys_sem_delete(mutex);
	sys_sem_delete(printID);
	return 0;
}

void addPhylo() {
	sys_sem_wait(mutex);
	if (current == MAX_PHYLOS) {
		printf("MAX PHYLOS REACHED\n");
	} else {
		status[current] = THINKING;
		s[current] = sys_create_anon_sem(0);
		alt[current] = sys_create_anon_sem(1);
		sys_sem_anon_open(s[current]);
		sys_sem_anon_open(alt[current]);

		char ** args = (char **) sys_malloc(3 * sizeof(char *));
		if (args == NULL) {
			return;
		}
		char * buf = (char *) sys_malloc(8);
		if (buf == NULL) {
			sys_free(args);
			return;
		}
		intToStr(current, buf);

		args[0] = "phylosophers";
		args[1] = buf;
		int fds[2] = {0, 1};
		pids[current] = sys_create_process("phylosophers", args, &philo, 0, fds);
		if (pids[current] <= 0) {
			printf("error creating philosopher. aborting\n");
			return;
		}

		current++;
	}
	sys_sem_post(mutex);
}

int philo(char ** num) {
	int i = atoi(num[1]);
	while (!last) {
		sys_sem_wait(alt[i]);
		think();
		forks(i);
		eat();
		put(i);
		sys_sem_post(alt[i]);
	}
	return 0;
}

void remove() {
	if (current == MIN_PHYLOS) {
		printf("MIN PHYLOS REACHED\n");
		return;
	}
	sys_sem_wait(alt[current - 1]);
	sys_sem_wait(mutex);

	current--;
	sys_kill_process(pids[current]);
	sys_sem_delete(s[current]);
	sys_sem_delete(alt[current]);

	sys_sem_post(mutex);
}

void think() {
	for (int i = 0; i < 5000000; i++)
		;
}

void forks(int i) {
	sys_sem_wait(mutex);
	status[i] = HUNGRY;
	test(i);
	sys_sem_post(mutex);
	sys_sem_wait(s[i]);
}

void eat() {
	for (int i = 0; i < 5000000; i++)
		;
	sys_sem_wait(printID);

	for (int i = 0; i < current; i++) {
		printf(status[i] == EATING ? "E " : ". ", 2);
	}
	printf("\n");
	sys_sem_post(printID);
}

void put(int i) {
	sys_sem_wait(mutex);
	status[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	sys_sem_post(mutex);
}

void test(int i) {
	if (status[i] == HUNGRY && status[LEFT] != EATING && status[RIGHT] != EATING) {
		status[i] = EATING;
		sys_sem_post(s[i]);
	}
}
