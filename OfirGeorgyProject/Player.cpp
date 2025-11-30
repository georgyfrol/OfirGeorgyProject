#include "Player.h"
#include "io_utils.h"
#include "Level.h"
#include "Color.h"
#include <iostream>
#include <cctype>   // for tolower function

using namespace std;

void Player::init(int startX, int startY, char sym, Color c, char kU, char kD, char kL, char kR, char kS, char kE) {
    x = startX;
    y = startY;
    symbol = sym;
    color = c;

    dir_x = 0;
    dir_y = 0;
    
    //using tolower function to always stay on lower case keys
    keyUp = tolower(kU);
    keyDown = tolower(kD);
    keyLeft = tolower(kL);
    keyRight = tolower(kR);
    keyStay = tolower(kS);
    keyDispose = tolower(kE);
}

void Player::setDirection(char key) {
    key = tolower(key);

    if (key == keyUp) {
        dir_x = 0; dir_y = -1;
    }
    else if (key == keyDown) {
        dir_x = 0; dir_y = 1;
    }
    else if (key == keyLeft) {
        dir_x = -1; dir_y = 0;
    }
    else if (key == keyRight) {
        dir_x = 1; dir_y = 0;
    }
    else if (key == keyStay) {
        dir_x = 0; dir_y = 0;
    }
}

void Player::erase() {
    gotoxy(x, y);
    cout << ' ';
}

void Player::draw() {
    setTextColor(color);
    gotoxy(x, y);
    cout << symbol;
    setTextColor(WHITE);
}

void Player::move(Level& level) { //players' movments with wrap-around
                      //HEIGHT and WIDTH are from Level.h 
    if (dir_x == 0 && dir_y == 0) return;

    int next_x = x + dir_x;
    int next_y = y + dir_y;

    if (next_x < 0)
        next_x = WIDTH - 1;
    else if (next_x >= WIDTH)
        next_x = 0;

    if (next_y < 0)
        next_x = HEIGHT - 1;
    else if (next_y >= HEIGHT)
        next_y = 0;

    char nextCell = level.getCharAt(next_x, next_y);

    if (nextCell == 'W') {
        dir_x = 0;
        dir_y = 0;
    }
    else {
        erase();
        x = next_x;
        y = next_y;
        draw();
    }
    
}