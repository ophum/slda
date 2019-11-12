#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>

#define fps 100000000 / 48 
int game_end = 0;

char screen[24][80] = {0};
char problems[][256] = {
	"buritokaiteburitoyomu\0", 
	"sumomomomomomomomonouti\0",
	"daremohannnousitekurenai\0",
	"inkujettopurinta-\0",
	"sekaiissyuuryokou\0",
	"madonosotohatumetaiame\0",
	"kyouseisyuuryou\0",
	"suimanihakatenakatta\0",
	"kinnnikuhauragiranai\0",
	"koutyasennmonntenn\0",
};

int problem = 0;
int cursor = 0;
int point = 0;

void key() {
	char c;
	c = getchar_unlocked();

	if(c == problems[problem][cursor]) {
		cursor++;
		point++;
	}else {
		point = 0;
	}

	if(problems[problem][cursor] == '\0') {
		problem++;
		cursor = 0;
	}

	if(problem >= sizeof(problems)/sizeof(*problems)) game_end = 1;
}

void disp() {
	struct timespec treq, trem;
	treq.tv_sec = (time_t)0;
	treq.tv_nsec = fps;
	int i, j;
	while(!game_end) {
		printf("\033[2J");
		printf("\033[1;1H");
		for(i = 0; i < strlen(problems[problem]); i++) {
			printf("\033[1;%dH", i + 1);
			printf("%c", problems[problem][i]);
		}
		printf("\033[2;1H");
		for(i = 0; i < cursor; i++) {
			printf("\033[2;%dH", i + 1);
			putchar(' ');
		}
		printf("^\n");
		nanosleep(&treq, &trem);
	}
}

int main() {
	pthread_t disp_thread;
	int disp_ret;

	game_end = 0;

	disp_ret = pthread_create(&disp_thread, NULL, (void *)disp, NULL);

	initscr();
	noecho();
	while(!game_end) {
		key();
	}
	echo();
	endwin();
	pthread_join(disp_thread, NULL);

	printf("point == %d\n", point);

	return 0;
}
