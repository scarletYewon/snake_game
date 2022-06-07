#include <queue>
#include <cstdlib>
#include <ncursesw/curses.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale.h>
#include <unistd.h>

#include "snake_ncurses.h"
#include "snake.h"

// Length, Growth, Poison, Gate
int MISSION[4][4] = {
    {6, 4, 2, 1},
    {7, 5, 2, 1},
    {8, 6, 3, 2},
    {9, 7, 4, 3},
};


void print_snake() {
	mvprintw(5, 8, " ____  ____  ____  ____  ____  ____  ____  ____  ____  ____ \n");
	mvprintw(6, 8, "||S ||||N ||||A ||||K ||||E ||||% ||||G ||||A ||||M ||||E ||\n");
	mvprintw(7, 8, "||__||||__||||__||||__||||__||||__||||__||||__||||__||||__||\n");
	mvprintw(8, 8, "|/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\|\n\n");
 	mvprintw(11, 8, "CPP Programming - Class 06                   PJM & LYW TEAM\n");
  	mvprintw(12, 2, "______________________________________________________________________________\n");
	  
	/*
	 ____  ____  ____  ____  ____  ____  ____  ____  ____  ____ 
	||S ||||N ||||A ||||K ||||E ||||% ||||G ||||A ||||M ||||E ||
	||__||||__||||__||||__||||__||||__||||__||||__||||__||||__||
	|/__\||/__\||/__\||/__\||/__\||/__\||/__\||/__\||/__\||/__\|
	*/
}

int show_menu ()
{
	int c=1, ch;
	initscr();

	erase();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	print_snake();
	attron(A_STANDOUT);
	mvprintw(15, 32, " Play Snake Game");
	attroff(A_STANDOUT);
	mvprintw(16, 34, " Exit Game");
	while(1) {
		ch=getch();
		if(ch=='\n') {
			return c;
		}
		else if(ch==KEY_UP) {
			if(c>1) {
				c--;
			}
			else {
				c=3;
			}
		}
		else if(ch==KEY_DOWN) {
			if(c<2) {
				c++;
			}
			else {
				c=1;
			}
		}
		switch(c) {
			case 1:
				erase();
				print_snake();
				attron(A_STANDOUT);
				mvprintw(15, 32, " Play Snake Game");
				attroff(A_STANDOUT);
				mvprintw(16, 34, " Exit Game");
				break;
			case 2:
				erase();
				print_snake();
				mvprintw(15, 32, " Play Snake Game");
				attron(A_STANDOUT);
				mvprintw(16, 34, " Exit Game");
				attroff(A_STANDOUT);
				break;
		}
	}
}

void show_gameClear()
{
	erase();
	timeout(TIMEOUT_GAME_OVER);
	mvprintw(10, 10, "★★You are Winner★★★\n");
    getch();
}

void print_nextStage(int stage)
{
	erase();
	mvprintw(10, 10, "NEXT STAGE(%d) >> \n", stage+1);
	mvprintw(11, 10, "Press N key... \n");
	refresh();


	char ch = NULL;
	do{
		sleep(1);
		ch = getch();
	}while(ch!='n');
}


void show_gameover(int a)
{
	erase();
	timeout(TIMEOUT_GAME_OVER);
	mvprintw(5, 8, " ____  ____  ____  ____  ____  ____  ____  ____  ____  ____ \n");
	mvprintw(6, 8, "||G ||||A ||||M ||||E ||||% ||||O ||||V ||||E ||||R ||||! ||\n");
	mvprintw(7, 8, "||__||||__||||__||||__||||__||||__||||__||||__||||__||||__||\n");
	mvprintw(8, 8, "|/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\||/__\\|\n\n");
  	mvprintw(12, 2, "______________________________________________________________________________\n");
	mvprintw(14, 8, "                             GAME OVER \n");

}

void print_score(Snake& s)
{
	for(int r=1; r<=7; r++) mvprintw(r,35, "|");
	for(int r=1; r<=7; r++) mvprintw(r,60, "|");
	for(int c=35; c<=60; c++) mvprintw(1,c, "-");
	for(int c=35; c<=60; c++) mvprintw(7,c, "-");

	mvprintw(2, 36, "*******SCORE BOARD******");


	mvprintw(3, 37, "B: %d", s.getscore());
	mvprintw(4, 37, "+: %d", s.getCntGrowth());
	mvprintw(5, 37, "-: %d", s.getCntPoison());
	mvprintw(6, 37, "G: %d", s.getCntGate());


	refresh();
}

void print_mission(Snake& s)
{
	for(int r=10; r<=16; r++) mvprintw(r,35, "|");
	for(int r=10; r<=16; r++) mvprintw(r,60, "|");
	for(int c=35; c<=60; c++) mvprintw(10,c, "-");
	for(int c=35; c<=60; c++) mvprintw(16,c, "-");

	mvprintw(11, 36, "******MISSION BOARD*****");

	int stg = s.getStage();

	if(MISSION[stg-1][0] <= s.getscore())
		mvprintw(12, 37, "B: %d (V)", MISSION[stg-1][0]);
	else
		mvprintw(12, 37, "B: %d (%d)", MISSION[stg-1][0], s.getscore());

	if(MISSION[stg-1][1] <= s.getCntGrowth())
		mvprintw(13, 37, "+: %d (V)", MISSION[stg-1][1]);
	else
		mvprintw(13, 37, "+: %d (%d)", MISSION[stg-1][1], s.getCntGrowth());

	if(MISSION[stg-1][2] <= s.getCntPoison())
		mvprintw(14, 37, "-: %d (V)", MISSION[stg-1][2]);
	else
		mvprintw(14, 37, "-: %d (%d)", MISSION[stg-1][2], s.getCntPoison());

	if(MISSION[stg-1][3] <= s.getCntGate())
		mvprintw(15, 37, "G: %d (V)", MISSION[stg-1][3]);
	else
		mvprintw(15, 37, "G: %d (%d)", MISSION[stg-1][3], s.getCntGate());


	refresh();
}


void classic_game(void)
{
	start_color();
	use_default_colors();
	init_pair(COLOR_SNAKE_HEAD, COLOR_RED, COLOR_RED);
	init_pair(COLOR_SNAKE_BODY, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(COLOR_ITEM_GROWTH, COLOR_GREEN, COLOR_GREEN);
	init_pair(COLOR_ITEM_POISON, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(COLOR_IMMUNEWALL, COLOR_BLACK, COLOR_BLACK);
	init_pair(COLOR_WALL, COLOR_BLACK, COLOR_BLACK);
	init_pair(COLOR_GATE, COLOR_BLUE, COLOR_BLUE);

	int stage=1;
	int ch, d;
	Snake S(stage);

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	timeout(TIMEOUT);
	curs_set(0);


	while(1) {
		ch=getch();
		if(ch=='q') {
			return;
		}
		else if(ch==KEY_UP || ch==KEY_DOWN || ch==KEY_RIGHT || ch==KEY_LEFT) {
			switch(ch) {
				case KEY_UP:
					d=UP;
					break;
				case KEY_DOWN:
					d=DOWN;
					break;
				case KEY_RIGHT:
					d=RIGHT;
					break;
				case KEY_LEFT:
					d=LEFT;
					break;
			}
			if(diff(S.getfdir(), d)!=2 && d!=S.getfdir()) {
				S.createNode(d);
			}
		}


		S.movesnake();
		S.render();


		print_score(S);
		print_mission(S);

		if(S.getcoll()) {
			timeout(TIMEOUT_LONG);
			getch();
			show_gameover(S.getscore());
			return;
		}

		if(S.getscore()<DEF_LEN_CLASSIC) {
			timeout(TIMEOUT_LONG);
			getch();
			show_gameover(S.getscore());
			return;
		}


		if(missionClear(stage, S.getscore(), S.getCntGrowth(), S.getCntPoison(), S.getCntGate())){
			if(stage==4) break;
			print_nextStage(stage);
			stage++;
			S = Snake(stage);
			continue;
		}

		attron(A_STANDOUT);
		mvprintw(23, 15, "PRESS 'Q' to EXIT BACK TO MENU.");
		attroff(A_STANDOUT);
		refresh();
	}
}

Point rand_point(std::deque<Cell> cells, std::deque<Item> items, std::deque<Cell> walls){
	Point p;
	int flag;
	srand(time(NULL));
	do {
		flag=0;
		p.row=(rand()%(MAX_ROW-1))+1;
		p.col=(rand()%(MAX_COL-1))+1;
		for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
			if(p.row==it->p.row && p.col==it->p.col) {
				flag=1;
				break;
			}
		}

		for(std::deque<Item>::iterator it=items.begin(); it!=items.end(); ++it){
			if(p.row==it->p.row && p.col==it->p.col) {
				flag=1;
				break;
			}
		}

		for(std::deque<Cell>::iterator it=walls.begin(); it!=walls.end(); ++it){
			if(p.row==it->p.row && p.col==it->p.col) {
				flag=1;
				break;
			}
		}

	}while(flag);
	return p;
}

Point rand_point(std::deque<Cell> cells, std::deque<Item> items, std::deque<Cell> walls, std::deque<Cell> gates)
{
	int index = 0;
	Point p;
	int flag;
	srand(time(NULL));

	do {
		flag=0;

		index = rand() % walls.size();
		p = walls[index].p;

		for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
			if(p.row==it->p.row && p.col==it->p.col) {
				flag=1;
				break;
			}
		}

		for(std::deque<Item>::iterator it=items.begin(); it!=items.end(); ++it){
			if(p.row==it->p.row && p.col==it->p.col) {
				flag=1;
				break;
			}
		}

		for(std::deque<Cell>::iterator it=gates.begin(); it!=gates.end(); ++it){
			if(p.row==it->p.row && p.col==it->p.col) {
				flag=1;
				break;
			}
		}

		for(std::deque<Cell>::iterator it=walls.begin(); it!=walls.end(); ++it){
			if(p.row==it->p.row && p.col==it->p.col && it->type==IMMUNEWALL) {
				flag=1;
				break;
			}
		}

	}while(flag);
	return p;
}


int rand_score(void) {
	struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);

	int rnd = rand();

	if(rnd%2) return 1;
	return -1;
}

int diff(int a, int b) {
	if((a-b)>0) {
		return a-b;
	}
	else {
		return b-a;
	}
}

int missionClear(int stage, int len, int item1, int item2, int gate)
{
	if(MISSION[stage-1][0]<=len && MISSION[stage-1][1]<=item1 && MISSION[stage-1][2]<=item2 && MISSION[stage-1][3]<=gate) return 1;

	return 0;
}
