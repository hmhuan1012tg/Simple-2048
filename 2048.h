//File "2048.h"

#pragma once

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include "console.h"

#define TILE_WIDTH 5 //Must be odd number
#define TILE_HEIGHT 3 //Must be odd number

#define TILE_NUM_X 4
#define TILE_NUM_Y 4

#define BOARD_WIDTH TILE_NUM_X*TILE_WIDTH + TILE_NUM_X + 1
#define BOARD_HEIGHT TILE_NUM_Y*TILE_HEIGHT + TILE_NUM_Y + 1

const int CENTRAL = TILE_WIDTH * TILE_HEIGHT / 2;
const int DISTANCE_X = TILE_WIDTH + 1;
const int DISTANCE_Y = TILE_HEIGHT + 1;

//Point struct
struct Point {
	int x, y;
};

const Point START_POINT = { TILE_WIDTH / 2 + 1, TILE_HEIGHT / 2 + 1 };

//Cell class
class Cell {
public: //Public section
	Cell(Point pos = { 0 , 0 }, char key = ' '); //Class constructor

	Point getPos() const; //Get cell position
	char getKey() const; //Get cell key
	bool setPos(int x, int y); //Set cell position
	bool setKey(char key); //Set cell key
private: //Private section
	Point m_pos;
	char m_key;
};

//Tile class
class Tile {
public: //Public section
	Tile(Point mainPos = START_POINT, int num = -1); //Class constructor

	bool drawTile(); //Print tile to screen
	bool eraseTile(); //Erase tile from screen

	bool isEmpty() const; //Check if a tile is empty or not
	Point getPos() const; //Get main position
	int getNum() const; //Get tile number
	void makeEmpty(); //Make tile empty

	friend bool operator==(const Tile& t1, const Tile& t2); //Overloading operator ==
	friend Tile operator+(const Tile& t1, const Tile& t2); //Overloading operator +

private: //Private section
	Cell m_cell[TILE_WIDTH*TILE_HEIGHT];
	int m_num;
	bool m_isDrawn;
};

class GameBoard {
public: //Public section
	GameBoard(); //Class constructor

	bool swipeUp(); //Swipe up
	bool swipeRight(); //Swipe right
	bool swipeDown(); //Swipe down
	bool swipeLeft(); //Swipe left

	void addTile(); //Add new non-empty tile randomly

	bool isFull() const; //Check if game board is full of tiles
	bool outOfMove() const; //Check if there is no valid move on board

	bool drawBoard(); //Draw game board
private: //Private section
	vector<vector<Tile>> m_tiles; //Tiles in board
	bool m_isChanged;
};

