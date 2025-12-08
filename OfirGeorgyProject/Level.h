#pragma once
#include <iostream>
#include <vector>
#include "Riddle.h"

const int HEIGHT = 25;
const int WIDTH = 80;

class Level {
	char map[HEIGHT][WIDTH + 1]; // +1 for null terminator if using strings
	
	// Door state tracking
	int door1KeysRequired;  // Number of keys still needed for Door 1 (starts at 2)
	bool door1Open;         // Whether Door 1 is open
	bool door2Open;         // Whether Door 2 is open (controlled by switches)
	vector<Riddle> riddles;

public:
	// initialize the hardcoded map
	void init(int levelNum);

	// Print the whole board (only called once at the start)
	void printLevel();
	
	// Draw doors with appropriate colors based on their state
	void drawDoors();

	// Returns the character at a specific position (for collision detection)
	char getCharAt(int x, int y);
	
	void setCharAt(int x, int y, char c);
	
	// Door state management
	bool isDoor1Open() const { return door1Open; }
	bool isDoor2Open() const { return door2Open; }
	bool tryUnlockDoor1();  // Returns true if door is now open
	void setDoor2Open(bool open) { door2Open = open; }
	
	// Check if switches are ON (for Door 2)
	bool checkSwitchesState() const;  // Returns true if both switches (S) are being held
	void addRiddle(int x, int y, string q, string a);
	const Riddle* getRiddle(int x, int y);
	void removeRiddle(int x, int y);
};