#pragma once
#include "Level.h"
#include "Player.h"
#include "Bomb.h"
#include <vector>
#include <string>
#include <list>
#include <fstream>

enum class GameMode {
    NORMAL,
    SAVE,
    LOAD
};

// Game Class (Handles Menu and Main Loop)
class Game {
private:
    Level level;
    Player p1;
    Player p2;
    std::list<Bomb> activeBombs;
    bool isRunning = true;
    bool gameActive = false;
    int currentLevelNum = 1;

    GameMode currentMode = GameMode::NORMAL;
    bool silentMode = false;
    long long gameCycle = 0; // cycle counter
    unsigned int randomSeed;

    // work with files
    std::ofstream stepsFileOut;
    std::ofstream resultFileOut;
    std::ifstream stepsFileIn;
    std::ifstream resultFileIn;

    struct LoadedStep {
        long long cycle;
        char key;
        std::string extraData;
    };
    std::vector<LoadedStep> loadedSteps;
    int currentStepIndex = 0;

    void initFiles(int argc, char* argv[]);
    void saveStep(char key, const std::string& extra = "");
    char loadStep();
    std::string loadRiddleAnswer();

    void saveResultEvent(const std::string& eventType, const std::string& details);
    void checkResultEvent(const std::string& expectedType, const std::string& expectedDetails);

public:
    // Game Logic
    int run(int argc, char* argv[]); // Returns false on fatal error
    bool runGame();	// Returns 0 on success, 1 on fatal error
    bool loadNextLevel();	// Returns false on fatal error
    void displayMenu();
    void displayInstructions();
    bool pauseGame();
    void handleRiddle(Player& p, int targetX, int targetY);
    void printHUD(int messageTimer, std::string displayMessage);
};