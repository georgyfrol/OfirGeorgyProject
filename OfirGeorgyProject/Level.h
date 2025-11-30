#pragma once
#include <iostream>

const int HEIGHT = 25;
const int WIDTH = 80;

class Level {
	char map[HEIGHT][WIDTH + 1]; // +1 for null terminator if using strings

public:
	// initialize the hardcoded map
	void init(int levelNum);

	// Print the whole board (only called once at the start)
	void printLevel();

	// Returns the character at a specific position (for collision detection)
	char getCharAt(int x, int y);
	
	void setCharAt(int x, int y, char c);

};