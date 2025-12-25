#include "Game.h"
#include "Player.h"
#include "Torch.h"
#include "io_utils.h"
#include <conio.h>  // For _kbhit and _getch
#include <windows.h> // For gotoxy and Sleep
#include <string> 
#include <cstdlib>  // For system("cls")


void Game::loadNextLevel() {
    // Increment to next level number
    currentLevelNum++;
    
    // Check if we've reached the end screen (Level 3)
    if (currentLevelNum == 3) {
        // Load Level 3 (End Screen)
        clear_screen();
        if (!level.init(3)) {
            gameActive = false;
            return;
        }
        level.printLevel();
        
        // End game sequence
        Sleep(2000);
        gameActive = false;
        return;
    }
    
    // Load next challenge level (Level 2)
    clear_screen();
    activeBombs.clear();  // Clear bombs from previous level
    
    // Preserve player inventory before reset
    char p1Inventory = p1.getInventory();
    char p2Inventory = p2.getInventory();
    
    // Initialize the new level
    if (!level.init(currentLevelNum)) {
        gameActive = false;
        return;
    }
    level.printLevel();
    
    // Reset both players to starting positions on the new map
    p1.init(5, 5, '$', Color::LIGHTGREEN, 'w', 'x', 'a', 'd', 's', 'e');
    p2.init(74, 5, '&', Color::LIGHTMAGENTA, 'i', 'm', 'j', 'l', 'k', 'o');
    
    // Restore inventory after reset
    p1.setInventory(p1Inventory);
    p2.setInventory(p2Inventory);
    
    p1.draw();
    p2.draw();
}

void Game::runGame() {
    gameActive = true;
    clear_screen();

    p1.setHealth(100);
    p1.setScore(0);
    p2.setHealth(100);
    p2.setScore(0);

    // Initialize starting level
    currentLevelNum = 1;
    if (!level.init(currentLevelNum)) return;
    level.printLevel();

    p1.init(5, 5, '$', Color::LIGHTGREEN, 'w', 'x', 'a', 'd', 's', 'e');
    p2.init(74, 5, '&', Color::LIGHTMAGENTA, 'i', 'm', 'j', 'l', 'k', 'o');

    p1.draw();
    p2.draw();

    int p1PrevX = -1, p1PrevY = -1;
    int p2PrevX = -1, p2PrevY = -1;
    bool p1PrevTorch = false;
    bool p2PrevTorch = false;
    bool forceUpdate = true; // To draw the first frame
    
    while (gameActive) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) { // ESC key
                if (!pauseGame()) gameActive = false;
                // Force screen update after pause
                forceUpdate = true;
            }
            else {
                char lowKey = tolower(key);
                int disposedX, disposedY;

                // Disposal Logic
                if (lowKey == 'e') {
                    char disposedItem = p1.dispose(level, disposedX, disposedY);
                    if (disposedItem == '@') activeBombs.emplace_back(disposedX, disposedY);
                }
                else if (lowKey == 'o') {
                    char disposedItem = p2.dispose(level, disposedX, disposedY);
                    if (disposedItem == '@') activeBombs.emplace_back(disposedX, disposedY);
                }
                else {
                    p1.setDirection(key);
                    p2.setDirection(key);
                }
            }
        }

        // Bomb ticking and damage logic 
        for (auto it = activeBombs.begin(); it != activeBombs.end(); ) {
            if (it->advance(level)) {
                // Check distance to players
                if (!p1.isFinished() && abs(p1.getX() - it->getX()) <= 2 && abs(p1.getY() - it->getY()) <= 2) {
                    p1.reduceHealth(50); // Damage 50
                }
                if (!p2.isFinished() && abs(p2.getX() - it->getX()) <= 2 && abs(p2.getY() - it->getY()) <= 2) {
                    p2.reduceHealth(50); // Damage 50
                }

                it = activeBombs.erase(it);
                forceUpdate = true;  // Explosion changes map, need to update light
            }
            else {
                ++it;
            }
        }

        // Switch logic
        if (level.checkSwitchesState()) {
            if (!level.isDoor2Open()) { // If door state changed
                level.setDoor2Open(true);
                level.drawDoors(); // Draw only if changed

                if (!level.isDoor2BonusGiven()) { // bonus for opening door with switches might be given only once
                    p1.addScore(75);
                    p2.addScore(75);
                    level.setDoor2BonusGiven(true);
                }
            }
        }
        else {
            if (level.isDoor2Open()) {
                level.setDoor2Open(false);
                level.drawDoors();
            }
        }

        // Move players
        char p1Result = ' ';
        if (!p1.isFinished()) {
            p1Result = p1.move(level);
            if (p1Result == '3') {
                p1.setFinished(true);
                p1.erase(level);            }
            else if (p1Result == '?') {
                handleRiddle(p1);
                forceUpdate = true;
            }
        }
        char p2Result = ' ';
        if (!p2.isFinished()) {
            p2Result = p2.move(level);
            if (p2Result == '3') {
                p2.setFinished(true);
                p2.erase(level);
            }
            else if (p2Result == '?') {
                handleRiddle(p2);
                forceUpdate = true;
            }
        }

        if (p1.getHealth() == 0 || p2.getHealth() == 0) {
            
            if (p1.getHealth() == 0)
                p1.reduceScore(100);
            else
                p2.reduceScore(100);

            clear_screen(); //message
            setTextColor(Color::LIGHTRED);
            cout << "\n\n\n\t\tPLAYER DIED! RESTARTING LEVEL...";
            cout << "\n\t\t-100 POINTS";
            Sleep(2000);
            setTextColor(Color::WHITE);

            // stage restart
            activeBombs.clear();
            level.init(currentLevelNum); // map restart
            level.printLevel();

            // players restart
            p1.init(5, 5, '$', Color::LIGHTGREEN, 'w', 'x', 'a', 'd', 's', 'e');
            p2.init(74, 5, '&', Color::LIGHTMAGENTA, 'i', 'm', 'j', 'l', 'k', 'o');
            p1.resetHealth();
            p2.resetHealth();

            p1.draw(); p2.draw();
            p1PrevX = -1; forceUpdate = true;
            continue;
        }

        if (p1.isFinished() && p2.isFinished()) {
            p1.addScore(200);
            p2.addScore(200);
            loadNextLevel();
            p1PrevX = -1; forceUpdate = true;
            continue;
        }

        // Lighting logic
        bool p1HasTorch = (p1.getInventory() == Torch::SYMBOL);
        bool p2HasTorch = (p2.getInventory() == Torch::SYMBOL);

        int drawP1x = p1.isFinished() ? -1 : p1.getX();
        int drawP1y = p1.isFinished() ? -1 : p1.getY();
        bool drawP1Torch = p1.isFinished() ? false : p1HasTorch;

        int drawP2x = p2.isFinished() ? -1 : p2.getX();
        int drawP2y = p2.isFinished() ? -1 : p2.getY();
        bool drawP2Torch = p2.isFinished() ? false : p2HasTorch;

        // Check if anything important for lighting has changed
        bool stateChanged =
            (p1.getX() != p1PrevX) || (p1.getY() != p1PrevY) ||
            (p2.getX() != p2PrevX) || (p2.getY() != p2PrevY) ||
            (p1HasTorch != p1PrevTorch) || (p2HasTorch != p2PrevTorch);

        if (level.isLevelDark()) {
            // Draw light only if something changed or it's the first frame
            if (stateChanged || forceUpdate) {
                level.updateLighting(p1.getX(), p1.getY(), p1HasTorch, p1.getSymbol(), p1.getColor(), p2.getX(), p2.getY(), p2HasTorch, p2.getSymbol(), p2.getColor());

                p1PrevX = p1.getX(); p1PrevY = p1.getY();
                p2PrevX = p2.getX(); p2PrevY = p2.getY();
                p1PrevTorch = p1HasTorch; p2PrevTorch = p2HasTorch;
                forceUpdate = false;
            }
        }
        else {
            if (!p1.isFinished()) p1.draw();
            if (!p2.isFinished()) p2.draw();
        }
        
        // Update springs
        for (auto& spring : level.getSprings()) {
            spring.updateCompression(p1, level);
            spring.updateCompression(p2, level);
        }

        if (p1Result == '?') {
            handleRiddle(p1);
            forceUpdate = true; // Screen was cleared after riddle, need to redraw
        }
        if (p2Result == '?') {
            handleRiddle(p2);
            forceUpdate = true;
        }

        // Inventory drawing
        gotoxy(0, HEIGHT);
        setTextColor(Color::LIGHTGREEN);
        cout << "P1: inventory:";
        setTextColor(Color::WHITE);
        cout << (p1.getInventory() ? p1.getInventory() : ' ');
        setTextColor(Color::LIGHTGREEN);
        cout << " HP:";
        setTextColor(Color::WHITE);
        cout << p1.getHealth() << "% ";
        setTextColor(Color::LIGHTGREEN);
        gotoxy(23, HEIGHT);
        cout << " Score:";
        setTextColor(Color::WHITE);
        cout << p1.getScore() << "  ";
        cout << "  ";
        //cout << "                  ";
        gotoxy(47, HEIGHT);
        setTextColor(Color::LIGHTMAGENTA);
        cout << "P2: inventory:";
        setTextColor(Color::WHITE);
        cout << (p2.getInventory() ? p2.getInventory() : ' ');
        setTextColor(Color::LIGHTMAGENTA);
        cout << " HP:";
        setTextColor(Color::WHITE);
        cout << p2.getHealth() << "% ";
        setTextColor(Color::LIGHTMAGENTA);
        gotoxy(70, HEIGHT);
        cout << " Score:";
        setTextColor(Color::WHITE);
        cout << p2.getScore() << "  ";
        cout << "  ";

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
    gotoxy(10, 15); cout << "-------- Doors logic --------";
    gotoxy(10, 16); cout << "Door 1: Requires 2 Keys";
    gotoxy(10, 17); cout << "Door 2: Requires both Switches (/)";
    gotoxy(10, 18); cout << "Door ?: Solve the Riddle to pass";
    gotoxy(10, 19); cout << "Door 3: Brings the players to the next map";
    gotoxy(10, 21); cout << "Press any key to return to the menu...";
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

bool Game::pauseGame() {
    gotoxy(0, HEIGHT);
    cout << "                                                                               ";
    gotoxy(11, HEIGHT);
    setTextColor(Color::YELLOW);
    cout << "Game paused. ESC: Continue, H: Home Menu, 8: Instructions";
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
            if (key == '8') {
                displayInstructions();

                clear_screen();
                level.printLevel();
                p1.draw();
                p2.draw();

                gotoxy(0, HEIGHT);
                setTextColor(Color::LIGHTGREEN);
                cout << "Player 1 inventory: ";
                setTextColor(Color::WHITE);
                cout << (p1.getInventory() ? p1.getInventory() : ' ');
                cout << "                                      ";
                setTextColor(Color::LIGHTMAGENTA);
                cout << "Player 2 inventory: ";
                setTextColor(Color::WHITE);
                cout << (p2.getInventory() ? p2.getInventory() : ' ');

                gotoxy(0, HEIGHT);
                cout << "                                                                               ";
                gotoxy(11, HEIGHT);
                setTextColor(Color::YELLOW);
                cout << "Game paused. ESC: Continue, H: Home Menu, 8: Instructions";
                setTextColor(Color::WHITE);
            }
        }
        Sleep(100);
    }
}
void Game::handleRiddle(Player& p) {
    int targetX = p.getX() + p.getDirX();
    int targetY = p.getY() + p.getDirY();

    if (targetX < 0) targetX = WIDTH - 1; else if (targetX >= WIDTH) targetX = 0;
    if (targetY < 0) targetY = HEIGHT - 1; else if (targetY >= HEIGHT) targetY = 0;

    const Riddle* r = level.getRiddle(targetX, targetY);
    if (!r) return;

    clear_screen();
    setTextColor(Color::CYAN);
    gotoxy(20, 8);  cout << "=== RIDDLE ===";
    setTextColor(Color::WHITE);
    gotoxy(20, 10); cout << r->getQuestion();
    gotoxy(20, 15); cout << "Answer: ";

    hideCursor();

    string input = "";
    while (true) {
        char c = _getch();
        if (c == 13) break; // Enter
        if (c == 8) { // Backspace
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        }
        else if (c >= 32 && c <= 126) {
            input += c;
            cout << c;
        }
    }

    gotoxy(20, 17);
    if (r->checkAnswer(input)) {
        setTextColor(Color::GREEN);
        cout << "CORRECT!";
        p.addScore(100);
        p.stop();
        Sleep(1000);

        level.setCharAt(targetX, targetY, ' ');
        level.removeRiddle(targetX, targetY);

        p.erase(level);
        p.setPosition(targetX, targetY);
    }
    else {
        setTextColor(Color::RED);
        cout << "WRONG! -10 HP";
        p.reduceHealth(10);
        p.reduceScore(10);
        Sleep(1000);
        p.stop();
    }

    setTextColor(Color::WHITE);

    clear_screen();
    level.printLevel();
    level.drawDoors();
    level.drawItems();
}