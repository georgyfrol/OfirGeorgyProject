#include "Level.h"
#include "io_utils.h"
#include <iostream>

using namespace std;

void Level::init() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map[i][j] = ' ';
        }
        map[i][WIDTH] = '\0';
    }

    for (int i = 0; i < WIDTH; i++) {
        map[0][i] = 'W';            // upper edge
        map[HEIGHT - 1][i] = 'W';   // lower edge
    }

    for (int i = 0; i < HEIGHT; i++) {
        map[i][0] = 'W';            // left edge
        map[i][WIDTH - 1] = 'W';    // right edge
    }

    
    for (int i = 5; i < 20; i++) {
        map[i][40] = 'W';
    }

    for (int i = 10; i < 30; i++) {
        map[10][i] = 'W';
    }

    for (int i = 50; i < 70; i++) {
        map[18][i] = 'W';
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
}

char Level::getCharAt(int x, int y) {
    if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) {
        return 'W';
    }
    return map[y][x];
}