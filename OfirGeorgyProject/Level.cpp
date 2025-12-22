#include "Level.h"
#include "io_utils.h"
#include "LevelData.h"
#include "Spring.h"
#include "Obstacle.h"
#include "Torch.h"
#include <iostream>
#include <cctype>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

void Level::init(int levelNum) {
    LevelData::load(levelNum, map);

    //Riddle initialization
    riddles.clear();
    vector<Riddle> pendingRiddles = LevelData::getRiddles(levelNum);
    int currentRiddleIndex = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '?') {
                   if (currentRiddleIndex < pendingRiddles.size()) {
                    Riddle r = pendingRiddles[currentRiddleIndex];
                    r.setPosition(x, y);
                    addRiddle(r);
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
        setDark(true);
        door1KeysRequired = 0;  // Door 1 doesn't exist in level 2
        door1Open = true;
        door2Open = false;  // Door 2 starts closed, requires switches
    }
    else {
        door1KeysRequired = 0;
        door1Open = true;
        door2Open = true;
    }
    
    // Detect and initialize springs from the map
    detectSprings();
    
    // Detect and initialize obstacles from the map
    detectObstacles();
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

void Level::drawItems() {
    // Scan the map for items and draw them with appropriate colors
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char c = map[y][x];
            if (c == '\\') {
                setTextColor(Color::YELLOW);
                gotoxy(x, y);
                cout << '\\';
            }
            else if (c == '/') {
                setTextColor(Color::YELLOW);
                gotoxy(x, y);
                cout << '/';
            }
            else if (c == '?') {
                setTextColor(Color::CYAN);
                gotoxy(x, y);
                cout << '?';
            }
            else if (c == 'K') {
                setTextColor(Color::YELLOW);
                gotoxy(x, y);
                cout << 'K';
            }
            else if (c == '@') {
                setTextColor(Color::LIGHTRED);
                gotoxy(x, y);
                cout << '@';
            }
            else if (c == '#') {
                setTextColor(Color::LIGHTCYAN);
                gotoxy(x, y);
                cout << '#';
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
    int openSwitches = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '/') {
                openSwitches++;
            }
        }
    }
    return (openSwitches >= 2);
}
void Level::addRiddle(Riddle r) {
    riddles.push_back(r);
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

void Level::detectSprings() {
    springs.clear();
    
    // Track which cells have been processed
    bool processed[HEIGHT][WIDTH] = {false};
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '#' && !processed[y][x]) {
                // Found a new spring cell, start building a spring
                Spring spring;
                
                // Check if it's horizontal or vertical by checking neighbors
                bool isHorizontal = false;
                bool isVertical = false;
                
                // Check right neighbor
                if (x + 1 < WIDTH && map[y][x + 1] == '#') {
                    isHorizontal = true;
                }
                // Check bottom neighbor
                else if (y + 1 < HEIGHT && map[y + 1][x] == '#') {
                    isVertical = true;
                }
                // Single cell spring - check which side has wall
                else {
                    // Check all four directions for wall
                    if (x - 1 >= 0 && map[y][x - 1] == 'W') {
                        isHorizontal = true;
                    }
                    else if (x + 1 < WIDTH && map[y][x + 1] == 'W') {
                        isHorizontal = true;
                    }
                    else if (y - 1 >= 0 && map[y - 1][x] == 'W') {
                        isVertical = true;
                    }
                    else if (y + 1 < HEIGHT && map[y + 1][x] == 'W') {
                        isVertical = true;
                    }
                    else {
                        // Default to horizontal if unclear
                        isHorizontal = true;
                    }
                }
                
                // Build the spring by collecting all consecutive '#' cells
                if (isHorizontal) {
                    spring.setOrientation(HORIZONTAL);
                    int startX = x;
                    // Collect all consecutive '#' in this row
                    while (startX < WIDTH && map[y][startX] == '#') {
                        spring.addCell(startX, y);
                        processed[y][startX] = true;
                        startX++;
                    }
                }
                else if (isVertical) {
                    spring.setOrientation(VERTICAL);
                    int startY = y;
                    // Collect all consecutive '#' in this column
                    while (startY < HEIGHT && map[startY][x] == '#') {
                        spring.addCell(x, startY);
                        processed[startY][x] = true;
                        startY++;
                    }
                }
                
                // Determine wall direction
                // Find which end of the spring is adjacent to a wall
                const vector<SpringCell>& cells = spring.getCells();
                if (cells.size() > 0) {
                    // Check first cell (one end)
                    int firstX = cells[0].x;
                    int firstY = cells[0].y;
                    bool firstHasWall = false;
                    int wallDirX = 0, wallDirY = 0;
                    
                    // Check last cell (other end)
                    int lastX = cells[cells.size() - 1].x;
                    int lastY = cells[cells.size() - 1].y;
                    bool lastHasWall = false;
                    
                    if (spring.getOrientation() == HORIZONTAL) {
                        // Check left side of first cell
                        if (firstX - 1 >= 0 && map[firstY][firstX - 1] == 'W') {
                            firstHasWall = true;
                            wallDirX = -1;
                            wallDirY = 0;
                        }
                        // Check right side of last cell
                        else if (lastX + 1 < WIDTH && map[lastY][lastX + 1] == 'W') {
                            lastHasWall = true;
                            wallDirX = 1;
                            wallDirY = 0;
                        }
                    }
                    else { // VERTICAL
                        // Check top side of first cell
                        if (firstY - 1 >= 0 && map[firstY - 1][firstX] == 'W') {
                            firstHasWall = true;
                            wallDirX = 0;
                            wallDirY = -1;
                        }
                        // Check bottom side of last cell
                        else if (lastY + 1 < HEIGHT && map[lastY + 1][lastX] == 'W') {
                            lastHasWall = true;
                            wallDirX = 0;
                            wallDirY = 1;
                        }
                    }
                    
                    // If no wall found at ends, check if spring is adjacent to wall
                    if (!firstHasWall && !lastHasWall) {
                        // Check all cells for adjacent walls
                        for (const auto& cell : cells) {
                            if (cell.x - 1 >= 0 && map[cell.y][cell.x - 1] == 'W') {
                                wallDirX = -1;
                                wallDirY = 0;
                                break;
                            }
                            if (cell.x + 1 < WIDTH && map[cell.y][cell.x + 1] == 'W') {
                                wallDirX = 1;
                                wallDirY = 0;
                                break;
                            }
                            if (cell.y - 1 >= 0 && map[cell.y - 1][cell.x] == 'W') {
                                wallDirX = 0;
                                wallDirY = -1;
                                break;
                            }
                            if (cell.y + 1 < HEIGHT && map[cell.y + 1][cell.x] == 'W') {
                                wallDirX = 0;
                                wallDirY = 1;
                                break;
                            }
                        }
                    }
                    
                    spring.setWallDirection(wallDirX, wallDirY);
                }
                
                springs.push_back(spring);
            }
        }
    }
}

void Level::detectObstacles() {
    obstacles.clear();
    
    // Track which cells have been processed
    bool processed[HEIGHT][WIDTH] = {false};
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == '*' && !processed[y][x]) {
                // Found a new obstacle cell, use flood-fill to collect all adjacent cells
                Obstacle obstacle;
                queue<pair<int, int>> toProcess;
                
                toProcess.push({x, y});
                processed[y][x] = true;
                
                while (!toProcess.empty()) {
                    pair<int, int> current = toProcess.front();
                    toProcess.pop();
                    
                    int cx = current.first;
                    int cy = current.second;
                    
                    obstacle.addCell(cx, cy);
                    
                    // Check all 4 adjacent directions (not diagonals)
                    int dx[] = {0, 1, 0, -1};
                    int dy[] = {-1, 0, 1, 0};
                    
                    for (int i = 0; i < 4; i++) {
                        int nx = cx + dx[i];
                        int ny = cy + dy[i];
                        
                        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                            if (map[ny][nx] == '*' && !processed[ny][nx]) {
                                processed[ny][nx] = true;
                                toProcess.push({nx, ny});
                            }
                        }
                    }
                }
                
                obstacles.push_back(obstacle);
            }
        }
    }
}

Obstacle* Level::getObstacleAt(int x, int y) {
    for (Obstacle& obstacle : obstacles) {
        if (obstacle.contains(x, y)) {
            return &obstacle;
        }
    }
    return nullptr;
}

void Level::updateLighting(int p1x, int p1y, bool p1Torch, int p2x, int p2y, bool p2Torch) {
    if (!isDark) return;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {//check if one of the players lits the cell
            bool litByP1 = p1Torch && Torch::isLit(j, i, p1x, p1y);
            bool litByP2 = p2Torch && Torch::isLit(j, i, p2x, p2y);

            if (litByP1 || litByP2) {
                gotoxy(j, i);
                char c = map[i][j];
                
                //coloring
                if (c == '!') {
                    setTextColor(Color::LIGHTRED);
                }
                else if (c == '@') {
                    setTextColor(Color::LIGHTRED);
                }
                else if (c == '#') {
                    setTextColor(Color::LIGHTCYAN);
                }
                else if (c == 'K' || c == '/' || c == '\\') {
                    setTextColor(Color::YELLOW);
                }
                else if (c == '?') {
                    setTextColor(Color::CYAN);
                }
                else if (c == '1') {
                    setTextColor(door1Open ? Color::GREEN : Color::RED);
                }
                else if (c == '2') {
                    setTextColor(door2Open ? Color::GREEN : Color::RED);
                }
                else if (c == '3') {
                    setTextColor(Color::GREEN);
                }
                else if (c == '*') {
                    setTextColor(Color::LIGHTGREY);
                }
                else {
                    setTextColor(Color::WHITE);
                }

                cout << c;
            }
            else {//if the cell dark it's a space
                gotoxy(j, i);
                cout << ' ';
            }
        }
    }
    setTextColor(Color::WHITE);
}