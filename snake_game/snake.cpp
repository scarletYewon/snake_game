#include <queue>
#include <cstdlib>
#include <ncursesw/curses.h>
#include <locale.h>
#include <ctime>
#include <fstream>
#include <iostream>

#include "snake_ncurses.h"
#include "snake.h"
#include "stage.h"


Snake::Snake(int s) : len(DEF_LEN_CLASSIC), stage(s)
{
	int i;
	Cell temp;

	growth = 0;
	poison = 0;
	gate = 0;

	setWall(stage);

	item_starttime = 0;
	gate_starttime = 0;
	
	fdir=RIGHT;
	temp.p=rand_point(cells, items, walls);

	Point tmpP = temp.p;
	tmpP.col += DEF_LEN_CLASSIC;

	if((temp.p.col+DEF_LEN_CLASSIC) >= MAX_ROW-1){
		temp.p.col -= (MAX_ROW-DEF_LEN_CLASSIC);
	}else if(isWall(tmpP)){
		temp.p.col -= MAX_ROW-DEF_LEN_CLASSIC;
	}

	for(i=0; i<len-1; i++) {
		temp.p.col+=i;
		if(temp.p.col+i >= MAX_COL-1){
			temp.p.col=MAX_COL-i-1;
		}
		cells.push_front(temp);
	}

	temp.p.col+=i;
	cells.push_front(temp);

	makeItem();
	coll=0;  
}



int Snake::getfdir() 
{
	return fdir;
}

void Snake::setfdir(int dir) 
{
	fdir=dir;
}

void Snake::createNode(int d) 
{
	fdir=d;
}

void Snake::makeItem() 
{
	item_starttime = time(NULL);
	srand(time(NULL));

	items.clear();
	Item tempItem;
	int cnt = (rand()%MAX_ITEM)+1;

	for(int i=0; i<cnt; i++){
		tempItem.p = rand_point(cells, items, walls);
		tempItem.points = rand_score();	
		items.push_back(tempItem);		
	}
	curitem.itemFlag = 0;
}

void Snake::eatItem()
{
	items.erase(curitem.t);
}

void Snake::makeGate()
{
	for(std::deque<Cell>::iterator snake=cells.begin(); snake!=cells.end(); ++snake){
		for(std::deque<Cell>::iterator gate=gates.begin(); gate!=gates.end(); ++gate){
			if(snake->p.row==gate->p.row && snake->p.col==gate->p.col) { 
				return;
			}
		}
	}

	gates.clear();
	gate_starttime = time(NULL);

	for(int i=0; i<2; i++){
		Cell temp;
		temp.p = rand_point(cells, items, walls, gates);
		gates.push_back(temp);		
	}

}

void Snake::movesnake() 
{
	Point f, b;
	Cell temp;
	f=cells.front().p;
	b=cells.back().p;

	switch(fdir) {
		case UP:
			f.row-=1;
			break;
		case DOWN:
			f.row+=1;
			break;
		case RIGHT:
			f.col+=1;
			break;
		case LEFT:
			f.col-=1;
			break;
	}
	temp.p=f;
	
	if(wallcollid() || collide()){
		coll=1;
		return;
	}

	cells.push_front(temp);

	bool itemFlag = false;
	for(std::deque<Item>::iterator it=items.begin(); it!=items.end(); ++it){
		if(cells.front().p.row==it->p.row && cells.front().p.col==it->p.col) { 
			itemFlag = true;

			curitem.t = it;
			curitem.p.row = it->p.row;
			curitem.p.col = it->p.col;
			curitem.itemFlag = it->points;
			break;
		}
	}

	bool gateFlag = false;
	Point gateP;

	for(std::deque<Cell>::iterator gate=gates.begin(); gate!=gates.end(); ++gate){
		if(cells.front().p.row==gate->p.row && cells.front().p.col==gate->p.col) { 
			gateFlag = true;	
		}else{
			gateP = gate->p;
		}
	}
	

	if(itemFlag){
		if(curitem.itemFlag == -1){
			cells.pop_back();
			cells.pop_back();
			poison++;
		}else if(curitem.itemFlag == 1){
			growth++;
		}
		eatItem();
	}else{	
		cells.pop_back();    
	}

	time_t cur_time;
	cur_time = time(NULL);

	if(cur_time-item_starttime >= ITEM_TIME){
		makeItem();
	}

	if(gateFlag){
		Cell temp;
		temp.p = gateP;

		if(gateP.row == 0){
			fdir = DOWN;
		}else if(gateP.row == MAX_ROW-1){
			fdir = UP;
		}else if(gateP.col == 0){
			fdir = RIGHT;
		}else if(gateP.col == MAX_COL-1){
			fdir = LEFT;
		}else{
		
			for(int i=0; i<4; i++){
				Point tmp = gateP;
				switch(fdir) {
					case UP:
						tmp.row-=1;
						break;
					case DOWN:
						tmp.row+=1;
						break;
					case RIGHT:
						tmp.col+=1;
						break;
					case LEFT:
						tmp.col-=1;
						break;
				}

				if(!isWall(tmp)) break;

				if(i==0) fdir = (fdir+2)%4;
				else fdir = (fdir+1)%4;
			}
		}

		cells.push_front(temp);
		cells.pop_back();
		gate++;

	}else{
		if(cells.size() >= APPEAR_GATE_LEN && (cur_time-gate_starttime >= GATE_TIME || gate_starttime==0)){
			makeGate();
		}
	}
	
}

void Snake::render()
{
	erase();
	
	for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
		if(it==cells.begin()){
			attron(COLOR_PAIR(COLOR_SNAKE_HEAD));
			mvprintw(it->p.row, it->p.col, "\u2B1B");
			attroff(COLOR_PAIR(COLOR_SNAKE_HEAD));
		}else{
			attron(COLOR_PAIR(COLOR_SNAKE_BODY));
			mvprintw(it->p.row, it->p.col, "\u2B1B");
			attroff(COLOR_PAIR(COLOR_SNAKE_BODY));
		}
	}
	
	for(std::deque<Item>::iterator it=items.begin(); it!=items.end(); ++it){
		if(it->points == 1){
			attron(COLOR_PAIR(COLOR_ITEM_GROWTH));
			mvprintw(it->p.row, it->p.col, "\u2B1B");
			attroff(COLOR_PAIR(COLOR_ITEM_GROWTH));
		}else if(it->points == -1){
			attron(COLOR_PAIR(COLOR_ITEM_POISON));
			mvprintw(it->p.row, it->p.col, "\u2B1B");
			attroff(COLOR_PAIR(COLOR_ITEM_POISON));
		}
	}

	for(std::deque<Cell>::iterator it=walls.begin(); it!=walls.end(); ++it){
		if(it->type == IMMUNEWALL){
			attron(COLOR_PAIR(COLOR_IMMUNEWALL));
			mvprintw(it->p.row, it->p.col, "\u2B1B");
			attroff(COLOR_PAIR(COLOR_IMMUNEWALL));
		}else if(it->type == WALL){
			attron(COLOR_PAIR(COLOR_WALL));
			mvprintw(it->p.row, it->p.col, "\u2B1B");
			attroff(COLOR_PAIR(COLOR_WALL));
		}
	}

	for(std::deque<Cell>::iterator it=gates.begin(); it!=gates.end(); ++it){
		attron(COLOR_PAIR(COLOR_GATE));
		mvprintw(it->p.row, it->p.col, "\u2B1B");
		attroff(COLOR_PAIR(COLOR_GATE));
	}
	refresh();
}

int Snake::collide() {
	for(std::deque<Cell>::iterator it=cells.begin(); it!=cells.end(); ++it){
		if(cells.front().p.row==it->p.row && cells.front().p.col==it->p.col && it!=cells.begin()) {
			return 1;
		}
	}
	return 0;
}

int Snake::wallcollid()
{
	if(isWall(cells.front().p)) 
		return 1;

	return 0;
}


void Snake::setWall(int stage)
{
	walls = setStageWall(stage);
}


int Snake::getcoll() 
{
	return coll;
}

int Snake::getscore() 
{
	return cells.size();
}

int Snake::isWall(Point p)
{
	for(std::deque<Cell>::iterator it=gates.begin(); it!=gates.end(); ++it){
		if(p.row==it->p.row && p.col==it->p.col){
			return 0;
		}
	}

	for(std::deque<Cell>::iterator it=walls.begin(); it!=walls.end(); ++it){
		if(p.row==it->p.row && p.col==it->p.col){
			return 1;
		}
	}

	return 0;
}

int Snake::getCntGrowth()
{
	return growth;
}

int Snake::getCntPoison()
{
	return poison;
}

int Snake::getCntGate()
{
	return gate;
}

int Snake::getStage()
{
	return stage;
}
