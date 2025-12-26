#pragma once
#include <iostream>
#include <vector>
#include "Riddle.h"
#include "Spring.h"
#include "Obstacle.h"
#include "Color.h"

const int HEIGHT = 25;
const int WIDTH = 80;

class Level {
	char map[HEIGHT][WIDTH + 1]; // +1 for null terminator if using strings
	
	// Door state tracking
	int door1KeysRequired;  // Number of keys still needed for Door 1 (starts at 2)
	bool door1Open;         // Whether Door 1 is open
	bool door2Open;         // Whether Door 2 is open (controlled by switches)
	vector<Riddle> riddles;
	vector<Spring> springs;  // All springs detected from the map
	vector<Obstacle> obstacles;  // All obstacles detected from the map
	bool isDark = false;
	bool door2BonusGiven = false;
	int legendX = -1;  // X coordinate of legend area (top-left corner marked by 'L')
	int legendY = -1;  // Y coordinate of legend area (top-left corner marked by 'L')

public:
	// initialize the hardcoded map
	bool init(int levelNum); // returns false if loading failed

	// Print the whole board (only called once at the start)
	void printLevel();
	
	// Draw doors and items with appropriate colors based on their state
	void drawDoors();
	void drawItems();


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
	void addRiddle(Riddle r);
	const Riddle* getRiddle(int x, int y);
	void removeRiddle(int x, int y);
	
	// Spring management
	void detectSprings();  // Scan map and create Spring objects from '#' characters
	const vector<Spring>& getSprings() const { return springs; }
	vector<Spring>& getSprings() { return springs; }
	
	// Obstacle management
	void detectObstacles();  // Scan map and create Obstacle objects from '*' characters
	const vector<Obstacle>& getObstacles() const { return obstacles; }
	vector<Obstacle>& getObstacles() { return obstacles; }
	Obstacle* getObstacleAt(int x, int y);  // Get obstacle at position, or nullptr if none
	
	void setDark(bool dark) { isDark = dark; }
	bool isLevelDark() const { return isDark; }
	void updateLighting(int p1x, int p1y, bool p1Torch, char p1Sym, Color p1Color, int p2x, int p2y, bool p2Torch, char p2Sym, Color p2Color);
	bool isDoor2BonusGiven() const { return door2BonusGiven; }
	void setDoor2BonusGiven(bool val) { door2BonusGiven = val; }
	int getDoor1KeysRequired() const { return door1KeysRequired; }
	int getLegendX() const { return legendX; }
	int getLegendY() const { return legendY; }
};
