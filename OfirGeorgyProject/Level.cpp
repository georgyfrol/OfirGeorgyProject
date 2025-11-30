#include "Level.h"
#include "io_utils.h"
#include "LevelData.h"
#include <iostream>
#include <cctype>

using namespace std;

void Level::init(int levelNum) {
    LevelData::load(levelNum, map);
}

void Level::printLevel() {
    setTextColor(Color::WHITE); 

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i);
        for (int j = 0; j < WIDTH; j++) {
            cout << map[i][j];
        }
    }
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