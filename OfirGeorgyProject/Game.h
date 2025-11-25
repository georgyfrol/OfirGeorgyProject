#pragma once
#include <iostream>
#include "io_utils.h"
#include "Player.h"
#include "Level.h"


using namespace std;

// --- Game Class (Handles Menu and Main Loop) ---

class Game {
private:
    bool isRunning = true;
    bool gameActive = false;

    Player p1, p2;
    Level level;

    // Game Logic
    void runGame();
    void displayInstructions();
    void displayMenu();
    void pauseGame(); // Will be needed for ESC key

public:
    // Main execution loop for the application
    void run();
};