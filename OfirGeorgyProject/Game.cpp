#include "Game.h"
#include "Player.h"
#include "Torch.h"
#include "io_utils.h"
#include <conio.h>  // For _kbhit and _getch
#include <windows.h> // For gotoxy and Sleep
#include <string> 
#include <cstdlib>  // For system("cls")

void Game::initFiles(int argc, char* argv[]) {
    currentMode = GameMode::NORMAL;
    silentMode = false;
    randomSeed = (unsigned int)time(0);

    if (argc > 1) {
        string modeStr = argv[1];
        if (modeStr == "-save") {
            currentMode = GameMode::SAVE;
            stepsFileOut.open("adv-world.steps");
            resultFileOut.open("adv-world.result");
            stepsFileOut << randomSeed << endl;
        }
        else if (modeStr == "-load") {
            currentMode = GameMode::LOAD;
            stepsFileIn.open("adv-world.steps");
            resultFileIn.open("adv-world.result");

            if (stepsFileIn.is_open()) {
                stepsFileIn >> randomSeed;
                long long t; char k;
                while (stepsFileIn >> t >> k) {
                    string extra = "";
                    if (k == '?') {
                        getline(stepsFileIn, extra);
                        if (!extra.empty() && extra[0] == ' ') extra.erase(0, 1);
                    }
                    loadedSteps.push_back({ t, k, extra });
                }
            }
            else {
                cout << "Error: Could not open adv-world.steps" << endl;
                exit(1);
            }
        }
    }

    if (argc > 2) {
        string silentStr = argv[2];
        if (silentStr == "-silent" && currentMode == GameMode::LOAD) {
            silentMode = true;
            isSilentMode = true;
        }
    }
    srand(randomSeed);
}

void Game::saveStep(char key, const string& extra) {
    if (currentMode == GameMode::SAVE) {
        stepsFileOut << gameCycle << " " << key;
        if (!extra.empty()) stepsFileOut << " " << extra;
        stepsFileOut << endl;
    }
}

char Game::loadStep() {
    if (currentStepIndex < loadedSteps.size()) {
        LoadedStep s = loadedSteps[currentStepIndex];
        if (s.cycle <= gameCycle) {
            currentStepIndex++;
            return s.key;
        }
    }
    return 0;
}

std::string Game::loadRiddleAnswer() {
    if (currentStepIndex < loadedSteps.size()) {
        if (loadedSteps[currentStepIndex].key == '?') {
            string answer = loadedSteps[currentStepIndex].extraData;
            currentStepIndex++;
            return answer;
        }
    }
    return "";
}
void Game::saveResultEvent(const string& eventType, const string& details) {
    if (currentMode == GameMode::SAVE) {
        resultFileOut << gameCycle << " " << eventType << " " << details << endl;
    }
}

void Game::checkResultEvent(const string& expectedType, const string& expectedDetails) {
    if (currentMode == GameMode::LOAD) {
        long long t; string type, details;
        if (resultFileIn >> t >> type) {
            getline(resultFileIn, details);
            if (!details.empty() && details[0] == ' ') details.erase(0, 1);

            bool match = (t == gameCycle) && (type == expectedType) && (details == expectedDetails);
            if (silentMode && !match) {
                bool oldSilent = isSilentMode; isSilentMode = false;
                cout << "TEST FAILED at cycle " << gameCycle << "!" << endl;
                cout << "Expected: " << expectedType << " " << expectedDetails << endl;
                cout << "Got:      " << t << " " << type << " " << details << endl;
                isSilentMode = oldSilent;
            }
        }
    }
}
string getHealthBar(int health) {
    string bar = "[";
    int hashes = health / 10;

    for (int i = 0; i < 10; i++) {
        if (i < hashes) bar += "#";
        else bar += ".";
    }
    bar += "]";
    return bar;
}
void printHealthBarColored(int health) {
    Color barColor;
    if (health > 70) barColor = Color::GREEN;
    else if (health > 20) barColor = Color::YELLOW;
    else barColor = Color::RED;

    setTextColor(Color::WHITE); cout << "[";
    setTextColor(barColor);

    int hashes = health / 10;
    for (int i = 0; i < 10; i++) {
        if (i < hashes) cout << "#";
        else cout << ".";
    }

    setTextColor(Color::WHITE); cout << "]";
}

bool Game::loadNextLevel() {
    currentLevelNum++;

    char p1Inventory = p1.getInventory();
    char p2Inventory = p2.getInventory();
    activeBombs.clear();

    clear_screen();
    string nextLevelFile = "adv-world_";
    if (currentLevelNum < 10) nextLevelFile += "0";
    nextLevelFile += to_string(currentLevelNum) + ".screen.txt";

    ifstream f(nextLevelFile);
    if (!f.good()) {
        if (!isSilentMode) {
            clear_screen();
            setTextColor(Color::GREEN);
            gotoxy(10, 10);
            cout << "CONGRATULATIONS! YOU HAVE FINISHED ALL LEVELS!";
            gotoxy(10, 12);
            cout << "Press any key to return to menu...";
            setTextColor(Color::WHITE);
            _getch();   
        }

        gameActive = false;
        return true;
    }
    f.close();

    clear_screen();
    if (!level.init(currentLevelNum)) {
        return false;
    }

    if (!level.isLevelDark()) {
        level.printLevel();
    }

    p1.init(5, 5, '$', Color::LIGHTCYAN, 'w', 'x', 'a', 'd', 's', 'e');
    p2.init(74, 5, '&', Color::LIGHTMAGENTA, 'i', 'm', 'j', 'l', 'k', 'o');

    p1.setInventory(p1Inventory);
    p2.setInventory(p2Inventory);

    p1.draw();
    p2.draw();

    return true;
}

void Game::printHUD(int messageTimer, string displayMessage) {
    int x = level.getLegendX();
    int y = level.getLegendY();

    // player 1
    gotoxy(x, y);
    setTextColor(Color::LIGHTCYAN); cout << "P1: Item:";
    setTextColor(Color::WHITE);     cout << (p1.getInventory() ? p1.getInventory() : ' ');
    setTextColor(Color::LIGHTCYAN); cout << " Score:";
    setTextColor(Color::YELLOW);    cout << p1.getScore();

    gotoxy(x, y + 1);
    setTextColor(Color::LIGHTCYAN); cout << "HP: ";
    setTextColor(Color::WHITE);     printHealthBarColored(p1.getHealth());

    // player 2
    gotoxy(x, y + 2);
    setTextColor(Color::LIGHTMAGENTA); cout << "P2: Item:";
    setTextColor(Color::WHITE);        cout << (p2.getInventory() ? p2.getInventory() : ' ');
    setTextColor(Color::LIGHTMAGENTA); cout << " Score:";
    setTextColor(Color::YELLOW);       cout << p2.getScore();

    gotoxy(x, y + 3);
    setTextColor(Color::LIGHTMAGENTA); cout << "HP: ";
    setTextColor(Color::WHITE);        printHealthBarColored(p2.getHealth());

    //print relevant message
    gotoxy(0, HEIGHT);
    if (messageTimer > 0) {
        setTextColor(Color::YELLOW);
        cout << "INFO: " << displayMessage;
        setTextColor(Color::WHITE);
    }
    else {
        cout << "                                                                               "; // clear line
    }
}

bool Game::runGame() {
    gameActive = true;
    clear_screen();
    Level::globalRiddleIndex = 0;
    int savedRiddleIndex = 0;
    gameCycle = 0;

    p1.setHealth(100);
    p1.setScore(0);
    p2.setHealth(100);
    p2.setScore(0);

    // Initialize starting level
    currentLevelNum = 1;
    if (!level.init(currentLevelNum)) {
        return false;  // Return error to exit from main
    }
    if (!level.isLevelDark()) {
        level.printLevel();
    }

    p1.init(5, 5, '$', Color::LIGHTCYAN, 'w', 'x', 'a', 'd', 's', 'e');
    p2.init(74, 5, '&', Color::LIGHTMAGENTA, 'i', 'm', 'j', 'l', 'k', 'o');

    p1.draw();
    p2.draw();

    int p1PrevX = -1, p1PrevY = -1;
    int p2PrevX = -1, p2PrevY = -1;
    bool p1PrevTorch = false;
    bool p2PrevTorch = false;
    bool forceUpdate = true; // To draw the first frame

    // Track previous legend values to prevent blinking
    int p1PrevHealth = -1;
    int p1PrevScore = -1;
    char p1PrevInventory = '\0';
    int p2PrevHealth = -1;
    int p2PrevScore = -1;
    char p2PrevInventory = '\0';
    bool legendNeedsUpdate = true; // Force update on first frame

    string displayMessage = "";
    int messageTimer = 0;

    while (gameActive) {
        gameCycle++;

        string frameMessage = "";
        char inputKey = 0;

        if (currentMode == GameMode::LOAD) {
            inputKey = loadStep();
            if (!loadedSteps.empty() && gameCycle > loadedSteps.back().cycle + 10) {
                cout << "The game was not finished\n" << endl;
                gameActive = false;
            }
        }
        else {
            if (_kbhit()) {
                inputKey = _getch();
                saveStep(inputKey);
            }
        }

        if (inputKey != 0) {
            if (inputKey == 27 && currentMode != GameMode::LOAD) {
                if (!pauseGame()) gameActive = false;
                forceUpdate = true;
            }
            else {
                char lowKey = tolower(inputKey);
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

                    // --- PLAYER 1 (WAXD) ---
                    if (lowKey == 'w') { // up
                        int nx = p1.getX(); int ny = p1.getY() - 1;
                        if (level.getCharAt(nx, ny) == '?') {
                            handleRiddle(p1, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p1.setDirection(inputKey);
                    }
                    else if (lowKey == 'x') { // down
                        int nx = p1.getX(); int ny = p1.getY() + 1;
                        if (level.getCharAt(nx, ny) == '?') {
                            p1.setDirection(0);
                            handleRiddle(p1, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p1.setDirection(inputKey);
                    }
                    else if (lowKey == 'a') { // left
                        int nx = p1.getX() - 1; int ny = p1.getY();
                        if (level.getCharAt(nx, ny) == '?') {
                            p1.setDirection(0);
                            handleRiddle(p1, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p1.setDirection(inputKey);
                    }
                    else if (lowKey == 'd') { // right
                        int nx = p1.getX() + 1; int ny = p1.getY();
                        if (level.getCharAt(nx, ny) == '?') {
                            p1.setDirection(0);
                            handleRiddle(p1, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p1.setDirection(inputKey);
                    }

                    // --- PLAYER 2 (IJML) ---
                    else if (lowKey == 'i') { // up
                        int nx = p2.getX(); int ny = p2.getY() - 1;
                        if (level.getCharAt(nx, ny) == '?') {
                            p2.setDirection(0);
                            handleRiddle(p2, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p2.setDirection(inputKey);
                    }
                    else if (lowKey == 'm') { // down
                        int nx = p2.getX(); int ny = p2.getY() + 1;
                        if (level.getCharAt(nx, ny) == '?') {
                            p2.setDirection(0);
                            handleRiddle(p2, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p2.setDirection(inputKey);
                    }
                    else if (lowKey == 'j') { // left
                        int nx = p2.getX() - 1; int ny = p2.getY();
                        if (level.getCharAt(nx, ny) == '?') {
                            p2.setDirection(0);
                            handleRiddle(p2, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p2.setDirection(inputKey);
                    }
                    else if (lowKey == 'l') { // right
                        int nx = p2.getX() + 1; int ny = p2.getY();
                        if (level.getCharAt(nx, ny) == '?') {
                            p2.setDirection(0);
                            handleRiddle(p2, nx, ny);
                            forceUpdate = true;
                            legendNeedsUpdate = true;
                        }
                        else p2.setDirection(inputKey);
                    }
                    else {
                        p1.setDirection(inputKey);
                        p2.setDirection(inputKey);
                    }
                }
            }
        }

        // Bomb ticking and damage logic 
        for (auto it = activeBombs.begin(); it != activeBombs.end(); ) {
            if (it->advance(level)) {
                // Check distance to players
                if (!p1.isFinished() && abs(p1.getX() - it->getX()) <= 2 && abs(p1.getY() - it->getY()) <= 2) {
                    p1.reduceHealth(50); // Damage 50
                    saveResultEvent("LIFE_LOST", "Player1 50");
                    checkResultEvent("LIFE_LOST", "Player1 50");
                }
                if (!p2.isFinished() && abs(p2.getX() - it->getX()) <= 2 && abs(p2.getY() - it->getY()) <= 2) {
                    p2.reduceHealth(50); // Damage 50
                    saveResultEvent("LIFE_LOST", "Player2 50");
                    checkResultEvent("LIFE_LOST", "Player2 50");
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
                displayMessage = "Door Unlocked";
                messageTimer = 30;
            }
        }
        else {
            if (level.isDoor2Open()) {
                level.setDoor2Open(false);
                level.drawDoors();
                displayMessage = "Door Closed";
                messageTimer = 30;
            }
        }

        // Move players
        char p1Result = ' ';
        if (!p1.isFinished()) {
            p1Result = p1.move(level, frameMessage, &p2);
            if (p1Result == '3') {
                p1.setFinished(true);
                p1.erase(level);
            }
        }
        char p2Result = ' ';
        if (!p2.isFinished()) {
            p2Result = p2.move(level, frameMessage, &p1);
            if (p2Result == '3') {
                p2.setFinished(true);
                p2.erase(level);
            }
        }

        if (!frameMessage.empty()) {
            displayMessage = frameMessage;
            messageTimer = 30;
        }

        if (p1.getHealth() == 0 || p2.getHealth() == 0) {

            string who = (p1.getHealth() == 0) ? "Player1" : "Player2";
            saveResultEvent("DEATH", who);
            checkResultEvent("DEATH", who);

            if (p1.getHealth() == 0)
                p1.reduceScore(100);
            else
                p2.reduceScore(100);

            if (!isSilentMode) {
                clear_screen(); //message
                setTextColor(Color::LIGHTRED);
                cout << "\n\n\n\t\tPLAYER DIED! RESTARTING LEVEL...";
                cout << "\n\t\t-100 POINTS";
                Sleep(2000);
                setTextColor(Color::WHITE);

                clear_screen();
            }

            // stage restart
            activeBombs.clear();
            Level::globalRiddleIndex = savedRiddleIndex;
            if (!level.init(currentLevelNum)) { // map restart
                gameActive = false;
                return false;  // Return error to exit from main
            }
            if (!level.isLevelDark()) {
                level.printLevel();
            }

            // players restart
            p1.init(5, 5, '$', Color::LIGHTCYAN, 'w', 'x', 'a', 'd', 's', 'e');
            p2.init(74, 5, '&', Color::LIGHTMAGENTA, 'i', 'm', 'j', 'l', 'k', 'o');
            p1.resetHealth();
            p2.resetHealth();

            p1.draw(); p2.draw();
            p1PrevX = -1; forceUpdate = true;
            legendNeedsUpdate = true; // Reset legend tracking
            p1PrevHealth = -1; p1PrevScore = -1; p1PrevInventory = '\0';
            p2PrevHealth = -1; p2PrevScore = -1; p2PrevInventory = '\0';
            continue;
        }

        if (p1.isFinished() && p2.isFinished()) {
            saveResultEvent("LEVEL_COMPLETE", to_string(currentLevelNum));
            checkResultEvent("LEVEL_COMPLETE", to_string(currentLevelNum));
            p1.addScore(200);
            p2.addScore(200);
            savedRiddleIndex = Level::globalRiddleIndex;
            if (!loadNextLevel()) {
                return false;  // Fatal error, exit from main
            }
            p1PrevX = -1; forceUpdate = true;
            legendNeedsUpdate = true; // Reset legend tracking
            p1PrevHealth = -1; p1PrevScore = -1; p1PrevInventory = '\0';
            p2PrevHealth = -1; p2PrevScore = -1; p2PrevInventory = '\0';
            continue;
        }

        // Lighting logic
        bool p1HasTorch = (p1.getInventory() == Torch::SYMBOL);
        bool p2HasTorch = (p2.getInventory() == Torch::SYMBOL);

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
                legendNeedsUpdate = true; // Legend needs to be redrawn after lighting update
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

        if (!silentMode) {
            // Check if legend values changed
            bool legendChanged =
                (p1.getHealth() != p1PrevHealth) ||
                (p1.getScore() != p1PrevScore) ||
                (p1.getInventory() != p1PrevInventory) ||
                (p2.getHealth() != p2PrevHealth) ||
                (p2.getScore() != p2PrevScore) ||
                (p2.getInventory() != p2PrevInventory);

            // HUD drawing - only update when values change or after lighting update
            if (legendChanged || legendNeedsUpdate || messageTimer > 0) {
                printHUD(messageTimer, displayMessage);
                p1PrevHealth = p1.getHealth();
                p1PrevScore = p1.getScore();
                p1PrevInventory = p1.getInventory();
                p2PrevHealth = p2.getHealth();
                p2PrevScore = p2.getScore();
                p2PrevInventory = p2.getInventory();
                legendNeedsUpdate = false;
            }

            if (messageTimer > 0) messageTimer--;

            setTextColor(Color::WHITE);
            Sleep(60);
        }
        else {
            if (messageTimer > 0)
                messageTimer--;
            Sleep(0);
        }
    }
    saveResultEvent("GAME_END", "TotalScore:" + to_string(p1.getScore() + p2.getScore()));
    checkResultEvent("GAME_END", "TotalScore:" + to_string(p1.getScore() + p2.getScore()));
    return true;  // Game completed successfully
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

int Game::run(int argc, char* argv[]) {
    initFiles(argc, argv);

    hideCursor();

    if (currentMode == GameMode::LOAD) {
        if (!runGame()) {
            return 1;
        }

        if (silentMode) {
            bool oldSilent = isSilentMode; isSilentMode = false;
            cout << "Test Finished." << endl;
            isSilentMode = oldSilent;
        }
    }
    else {
        while (isRunning) {
            displayMenu();

            // Non-blocking input check for menu selection
            while (!_kbhit()) {
                Sleep(50);
            }

            char choice = _getch();

            switch (choice) {
            case '1': // Start New Game
                if (!runGame()) {
                    return 1;  // Fatal error
                }
                break;
            case '2': // Toggle Colors
                setColorMode(!isColorMode());
                break;
            case '8': // Instructions
                displayInstructions();
                break;
            case '9': // EXIT
                isRunning = false;
                break;
            default:
                break;
            }
        }
    }

    if (stepsFileOut.is_open()) stepsFileOut.close();
    if (resultFileOut.is_open()) resultFileOut.close();
    if (stepsFileIn.is_open()) stepsFileIn.close();
    if (resultFileIn.is_open()) resultFileIn.close();

    // Clean exit
    clear_screen();
    if (!isSilentMode) {
        gotoxy(0, 0);
        cout << "Program finished normally." << endl;
    }
    return 0;
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
                if (!level.isLevelDark()) {
                    level.printLevel();
                }
                p1.draw();
                p2.draw();

                gotoxy(0, HEIGHT);
                setTextColor(Color::LIGHTCYAN);
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
void Game::handleRiddle(Player& p, int targetX, int targetY) {

    const Riddle* r = level.getRiddle(targetX, targetY);
    if (r == nullptr) return;

    string input = "";

    if (currentMode == GameMode::LOAD) {
        input = loadRiddleAnswer();
    }
    else {
        if (isSilentMode) return;

        clear_screen();
        setTextColor(Color::CYAN);
        gotoxy(20, 8);  cout << "=== RIDDLE ===";
        setTextColor(Color::WHITE);
        gotoxy(20, 10); cout << r->getQuestion();
        gotoxy(20, 15); cout << "Answer: ";

        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = TRUE;
        SetConsoleCursorInfo(consoleHandle, &info);

        while (true) {
            char c = _getch();
            if (c == 13) // enter
                break;
            if (c == 8) { // backspace
                if (!input.empty()) {
                    input.pop_back(); cout << "\b \b";
                }
            }
            else if (c >= 32 && c <= 126) {
                input += c; cout << c;
            }
        }

        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);

        saveStep('?', input);
    }

    bool correct = r->checkAnswer(input);
    string resultStr = correct ? "CORRECT" : "WRONG";

    string q = r->getQuestion();
    size_t pos = 0;
    while ((pos = q.find('\n', pos)) != string::npos) {
        q.replace(pos, 1, "\\n");
        pos += 2;
    }

    saveResultEvent("RIDDLE", q + "|" + input + "|" + resultStr);
    checkResultEvent("RIDDLE", q + "|" + input + "|" + resultStr);

    if (correct) {
        if (!silentMode) { setTextColor(Color::GREEN); cout << " CORRECT! + 100 POINTS"; Sleep(1000); }
        p.addScore(100);

        level.setCharAt(targetX, targetY, ' ');
        level.removeRiddle(targetX, targetY);
    }
    else {
        if (!silentMode) { setTextColor(Color::RED); cout << " WRONG! - 10 HP"; Sleep(1000); }
        p.reduceHealth(10);
        string pName = (p.getSymbol() == '$') ? "Player1" : "Player2";
        saveResultEvent("LIFE_LOST", pName + " 10");
        checkResultEvent("LIFE_LOST", pName + " 10");
    }

    if (!silentMode) {
        setTextColor(Color::WHITE);
        clear_screen();
        if (!level.isLevelDark()) {
            level.printLevel();
        }
        level.drawDoors();
        level.drawItems();
    }
}