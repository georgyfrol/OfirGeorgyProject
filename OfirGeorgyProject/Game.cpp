#include "Game.h"
#include <conio.h>  // For _kbhit and _getch
#include <windows.h> // For gotoxy and Sleep
#include <cstdlib>  // For system("cls")

// --- Console Utility Definitions ---

// Moves the cursor to (x, y) coordinates for precise printing.
void Game::gotoxy(int x, int y) {
    COORD dwCursorPosition;
    dwCursorPosition.X = x;
    dwCursorPosition.Y = y;

    cout.flush();

    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

// Clears the console screen.
void Game::clrscr() {
    system("cls");
}

// --- Game Logic Implementations ---

void Game::runGame() {
    gameActive = true;
    // Placeholder for the main game loop from Exercise 1
    clrscr();
    gotoxy(25, 12);
    cout << "--- Game Started (Placeholder) ---";
    gotoxy(25, 14);
    cout << "Press ESC to pause...";

    while (gameActive) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) { // ESC key
                // TODO: Replace with call to pauseGame() once implemented
                gameActive = false; // Exit game loop for now
            }
        }
        Sleep(100);
    }
}

void Game::displayInstructions() {
    clrscr();
    gotoxy(10, 3); cout << "--- Instructions and Keys ---";
    gotoxy(10, 5); cout << "P1: W/A/S/D (UP/LEFT/STAY/RIGHT) | P2: I/J/K/L (UP/LEFT/STAY/RIGHT)";
    gotoxy(10, 6); cout << "Elements: Wall (W), Key (K), Door (1-9), Spring (#), Obstacle (*), Torch (!)";
    gotoxy(10, 8); cout << "Press any key to return to the menu...";
    _getch(); // Wait for a key press
}

void Game::displayMenu() {
    clrscr();
    gotoxy(30, 5);  cout << "--- MTA Text Adventure World ---";
    gotoxy(30, 8);  cout << "(1) Start a new game";
    gotoxy(30, 9);  cout << "(8) Present instructions and keys";
    gotoxy(30, 10); cout << "(9) EXIT";
    gotoxy(30, 13); cout << "Enter choice: ";
}

void Game::run() {
    while (isRunning) {
        displayMenu();

        // Non-blocking input check for menu selection
        while (!_kbhit()) {
            Sleep(50);
        }

        char choice = _getch();

        switch (choice) {
        case '1': // Start New Game
            runGame();
            break;
        case '8': // Instructions
            displayInstructions();
            break;
        case '9': // EXIT
            isRunning = false;
            break;
        default:
            // Invalid choice is ignored; loop repeats
            break;
        }
    }

    // Clean exit
    clrscr();
    gotoxy(0, 0);
    cout << "Program finished normally." << endl;
}

// Placeholder for pause logic (will be implemented in a later step)
void Game::pauseGame() {}