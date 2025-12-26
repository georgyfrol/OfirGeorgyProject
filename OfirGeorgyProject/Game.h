#pragma once
#include <iostream>
#include "io_utils.h"
#include "Player.h"
#include "Level.h"
#include "Bomb.h"
#include "Spring.h"
#include <vector>
#include <string>


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

    bool runGame();  // Returns false on fatal error
    void displayInstructions();
    void displayMenu();
    bool pauseGame();
    bool loadNextLevel();  // Returns false on fatal error
    void handleRiddle(Player& p);

public:
    // Main execution loop for the application
    int run();  // Returns 0 on success, 1 on fatal error
};