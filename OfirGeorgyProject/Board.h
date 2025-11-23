#pragma once
#include <iostream>

// The board size is fixed for Ex1
const int HEIGHT = 25;
const int WIDTH = 80;

class Board {
	// 2D array for the map. 
	// We use 'char' to represent walls ('W'), spaces (' '), etc.
	char map[HEIGHT][WIDTH + 1]; // +1 for null terminator if using strings

public:
	// Initialize the hardcoded map
	void init();

	// Print the whole board (only called once at the start)
	void printBoard();

	// Returns the character at a specific position (for collision detection)
	char getCharAt(int x, int y);
};