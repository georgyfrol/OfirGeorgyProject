#pragma once
#include <iostream>

using namespace std;

// --- Game Class (Handles Menu and Main Loop) ---

class Game {
private:
    bool isRunning = true;
    bool gameActive = false;

    // Console Utility Functions (Defined in Game.cpp)
    void gotoxy(int x, int y);
    void clrscr();

    // Game Logic
    void runGame();
    void displayInstructions();
    void displayMenu();
    void pauseGame(); // Will be needed for ESC key

public:
    // Main execution loop for the application
    void run();
};