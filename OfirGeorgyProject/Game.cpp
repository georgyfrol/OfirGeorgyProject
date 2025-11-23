#include "Game.h"
#include "io_utils.h"
#include <conio.h>  // For _kbhit and _getch
#include <windows.h> // For gotoxy and Sleep
#include <cstdlib>  // For system("cls")


// --- Game Logic Implementations ---

void Game::runGame() {
    gameActive = true;
    // Placeholder for the main game loop from Exercise 1
    clear_screen(); 

    p1.init(5, 5, '$', 'w', 'x', 'a', 'd', 's', 'e');
    p2.init(70, 10, '&', 'i', 'm', 'j', 'l', 'k', 'o');

    p1.draw();
    p2.draw();

    while (gameActive) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) { // ESC key
                // TODO: Replace with call to pauseGame() once implemented
                gameActive = false; // Exit game loop for now
            }
            else {
                p1.setDirection(key);
                p2.setDirection(key);
            }
        }
        p1.move();
        p2.move();
        Sleep(100);
    }
}

void Game::displayInstructions() {
    clear_screen();
    gotoxy(10, 3); cout << "--- Instructions and Keys ---";
    gotoxy(10, 5); cout << "P1: W/X/A/S/D/E (UP/DOWN/LEFT/STAY/RIGHT/DISPOSE) | P2: I/M/J/K/L/O (UP/DOWN/LEFT/STAY/RIGHT/DISPOS)";
    gotoxy(10, 6); cout << "Elements: Wall (W), Key (K), Door (1-9), Spring (#), Obstacle (*), Torch (!)";
    gotoxy(10, 8); cout << "Press any key to return to the menu...";
    _getch(); // Wait for a key press
}

void Game::displayMenu() {
    clear_screen();
    gotoxy(30, 5);  cout << "--- MTA Text Adventure World ---";
    gotoxy(30, 8);  cout << "(1) Start a new game";
    gotoxy(30, 9);  cout << "(8) Present instructions and keys";
    gotoxy(30, 10); cout << "(9) EXIT";
    gotoxy(30, 13); cout << "Enter choice: ";
}

void Game::run() {
    hideCursor();
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
    clear_screen();
    gotoxy(0, 0);
    cout << "Program finished normally." << endl;
}

// Placeholder for pause logic (will be implemented in a later step)
void Game::pauseGame() {}