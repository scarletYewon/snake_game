#ifndef SNAKE_H
#define SNAKE_H


class Snake{
protected:
	int len;
	int stage;

	time_t item_starttime;	
	time_t gate_starttime;

	std::deque<Cell> cells;
	std::deque<Item> items;
	std::deque<Cell> walls;
	std::deque<Cell> gates;
	
	CurrentItem curitem;
	
	int fdir;				
	int score, coll;		

	int poison;
	int growth;
	int gate;

public:
	Snake(int s=1);	
	void createNode(int d);	
	
	void makeItem();	
	void eatItem();		
	void movesnake();	
	void render();		
	int collide();

	void setWall(int stage);
	int wallcollid();
	void makeGate();

	int getscore();
	int getcoll();
	int getfdir();
	void setfdir(int);

	int getCntGrowth();
	int getCntPoison();
	int getCntGate();
	int getStage();

	int isWall(Point p);
	
};

#endif
