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

    level.init(1);
    level.printLevel();

    p1.init(5, 5, '$', Color::LIGHTGREEN, 'w', 'x', 'a', 'd', 's', 'e');
    p2.init(74, 5, '&', Color::LIGHTMAGENTA, 'i', 'm', 'j', 'l', 'k', 'o');

    p1.draw();
    p2.draw();

    while (gameActive) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) { // ESC key
                bool resume = pauseGame();
                if (resume == false)
                    gameActive = false;
            }
            else {
                char lowKey = tolower(key);
                int disposedX, disposedY; // Variables to capture the item's disposal location

                // --- Updated Disposal Logic ---
                if (lowKey == 'e') {
                    char disposedItem = p1.dispose(level, disposedX, disposedY);
                    if (disposedItem == '@') {
                        // If Player 1 disposed a Bomb, add it to the active list
                        activeBombs.emplace_back(disposedX, disposedY);
                    }
                }
                else if (lowKey == 'o') {
                    char disposedItem = p2.dispose(level, disposedX, disposedY);
                    if (disposedItem == '@') {
                        // If Player 2 disposed a Bomb, add it to the active list
                        activeBombs.emplace_back(disposedX, disposedY);
                    }
                }
                // --- End Updated Disposal Logic ---
                else {
                    p1.setDirection(key);
                    p2.setDirection(key);
                }
            }
        }

        // --- Bomb Ticking Logic ---
        // Advance all active bombs and remove exploded ones.
        for (auto it = activeBombs.begin(); it != activeBombs.end(); ) {
            // Note: If you add Score/Lives (Ex 2), explosion logic must check if players 
            // were hit inside Bomb::explode or here, and reduce lives/score.
            if (it->advance(level)) {
                // Bomb exploded, removing it from the list
                it = activeBombs.erase(it);
            }
            else {
                ++it;
            }
        }
        // --- End Bomb Ticking Logic ---

        p1.move(level);
        p2.draw();
        p2.move(level);
        p1.draw();

        gotoxy(0, HEIGHT);
        setTextColor(Color::LIGHTGREEN);
        cout << "Player 1 inventory: ";
        setTextColor(Color::WHITE);
        cout << (p1.getInventory() ? p1.getInventory() : ' ');
        cout << "                                      "; // Padding reduced slightly
        setTextColor(Color::LIGHTMAGENTA);
        cout << "Player 2 inventory: ";
        setTextColor(Color::WHITE);
        cout << (p2.getInventory() ? p2.getInventory() : ' ');
        Sleep(100);
    }
}
void Game::displayInstructions() {
    clear_screen();
    gotoxy(10, 3);  cout << "--- Instructions and Keys ---";
    gotoxy(10, 5);  cout << "      Player 1    Player 2";
    gotoxy(10, 6);  cout << "UP       W           I";
    gotoxy(10, 7);  cout << "DOWN     X           M";
    gotoxy(10, 8);  cout << "LEFT     A           J";
    gotoxy(10, 9);  cout << "RIGHT    D           L";
    gotoxy(10, 10); cout << "STAY     S           K";
    gotoxy(10, 11); cout << "DISPOSE  E           O";
    gotoxy(10, 13); cout << "Elements: Wall (W), Key (K), Door (1-9), Spring (#), Obstacle (*), Torch (!)";
    gotoxy(10, 15); cout << "Press any key to return to the menu...";
    _getch(); // Wait for a key press
}

void Game::displayMenu() {
    clear_screen();
    gotoxy(30, 5);  cout << "--- MTA Text Adventure World ---";
    gotoxy(30, 8);  cout << "(1) Start a new game";
    gotoxy(30, 9);  cout << "(2) Toggle Colors:" << (isColorMode() ? "ON" : "OFF");
    gotoxy(30, 10); cout << "(8) Present instructions and keys";
    gotoxy(30, 11); cout << "(9) EXIT";
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
        case '2': //Toggle Colors
            setColorMode(!isColorMode());
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
bool Game::pauseGame() {
    gotoxy(0, HEIGHT);
    cout << "                                                                               ";
    gotoxy(3, HEIGHT);
    setTextColor(Color::YELLOW);
    cout << "Game paused, press ESC again to continue or H to go back to the main menu";
    setTextColor(Color::WHITE);

    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) {
                gotoxy(3, HEIGHT);
                cout << "                                                                               ";
                return true;
            }
            char lowKey = tolower(key);
            if (lowKey == 'h') return false;
        }
        Sleep(100);
    }
}