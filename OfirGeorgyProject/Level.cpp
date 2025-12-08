#include "Level.h"
#include "io_utils.h"
#include "LevelData.h"
#include <iostream>
#include <cctype>
#include <vector>

using namespace std;

void Level::init(int levelNum) {
    LevelData::load(levelNum, map);

    riddles.clear();
    vector<RiddleDataRaw> rawRiddles = LevelData::getRiddles(levelNum);
    int currentRiddleIndex = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '?') {
                   if (currentRiddleIndex < rawRiddles.size()) {
                    RiddleDataRaw data = rawRiddles[currentRiddleIndex];
                    addRiddle(x, y, data.question, data.answer);
                    currentRiddleIndex++;
                }
            }
        }
    }
    
    // Initialize door states based on level
    if (levelNum == 1) {
        door1KeysRequired = 2;  // Door 1 requires 2 keys
        door1Open = false;
        door2Open = true;  // Door 2 doesn't exist in level 1
    }
    else if (levelNum == 2) {
        door1KeysRequired = 0;  // Door 1 doesn't exist in level 2
        door1Open = true;
        door2Open = false;  // Door 2 starts closed, requires switches
    }
    else {
        door1KeysRequired = 0;
        door1Open = true;
        door2Open = true;
    }
}

void Level::printLevel() {
    setTextColor(Color::WHITE); 

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i);
        for (int j = 0; j < WIDTH; j++) {
            cout << map[i][j];
        }
    }
    
    // Draw doors with colors after printing the level
    drawDoors();
}

void Level::drawDoors() {
    // Scan the map for doors and draw them with appropriate colors
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char c = map[y][x];
            if (c == '1') {
                // Door '1' - RED if locked, GREEN if open
                if (door1Open) {
                    setTextColor(Color::GREEN);
                } else {
                    setTextColor(Color::RED);
                }
                gotoxy(x, y);
                cout << '1';
            }
            else if (c == '2') {
                // Door '2' - RED if locked, GREEN if open
                if (door2Open) {
                    setTextColor(Color::GREEN);
                } else {
                    setTextColor(Color::RED);
                }
                gotoxy(x, y);
                cout << '2';
            }
            else if (c == '3') {
                // Door '3' - Always GREEN (always open/available)
                setTextColor(Color::GREEN);
                gotoxy(x, y);
                cout << '3';
            }
        }
    }
    setTextColor(Color::WHITE);  // Reset to white
}

char Level::getCharAt(int x, int y) {
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) {
        return 'W';
    }
    return map[y][x];
}

void Level::setCharAt(int x, int y, char c) {
    if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
        map[y][x] = c;
        gotoxy(x, y);
        cout << c;
    }
}

bool Level::tryUnlockDoor1() {
    if (door1KeysRequired > 0) {
        door1KeysRequired--;
        if (door1KeysRequired == 0) {
            door1Open = true;
            return true;
        }
    }
    return false;
}

bool Level::checkSwitchesState() const {
    // Scan map for switches (S) and check if players are on them
    // This will be called from Game class with player positions
    // For now, return false - will be implemented in Game class
    return false;
}
void Level::addRiddle(int x, int y, string q, string a) {
    riddles.push_back(Riddle(x, y, q, a));
}

const Riddle* Level::getRiddle(int x, int y) {
    for (const auto& r : riddles) {
        if (r.getX() == x && r.getY() == y) return &r;
    }
    return nullptr;
}

void Level::removeRiddle(int x, int y) {
    for (auto it = riddles.begin(); it != riddles.end(); ++it) {
        if (it->getX() == x && it->getY() == y) {
            riddles.erase(it);
            return;
        }
    }
}