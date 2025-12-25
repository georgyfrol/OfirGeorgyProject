#pragma once
#include <iostream>
#include "io_utils.h"
#include "Player.h"
#include "Level.h"
#include "Bomb.h"
#include "Spring.h"
#include <vector>
#include <string>


using namespace std;

string getHealthBar(int health);
void printHealthBarColored(int health);

// Game Class (Handles Menu and Main Loop)
class Game {
private:
    bool isRunning = true;
    bool gameActive = false;
    int currentLevelNum;

    Player p1, p2;
    Level level;
    std::vector<Bomb> activeBombs;
    // Game Logic

    void runGame();
    void displayInstructions();
    void displayMenu();
    bool pauseGame();
    void loadNextLevel();
    void handleRiddle(Player& p);

public:
    // Main execution loop for the application
    void run();
};