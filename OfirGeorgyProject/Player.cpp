#include "Player.h"
#include "io_utils.h"
#include "Level.h"
#include <iostream>
#include <cctype>   // for tolower function

using namespace std;

void Player::init(int startX, int startY, char sym, char kU, char kD, char kL, char kR, char kS, char kE) {
    x = startX;
    y = startY;
    symbol = sym;

    dir_x = 0;
    dir_y = 0;

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
    gotoxy(x, y);
    cout << symbol;
}

void Player::move() {
    if (dir_x == 0 && dir_y == 0) return;

    erase();

    x += dir_x;
    y += dir_y;
    
    if (x < 1) {
        x = WIDTH - 1;
    }
    else if (x >= WIDTH) {
        x = 1;
    }

    if (y < 1) {
        y = HEIGHT - 1;
    }
    else if (y >= HEIGHT) {
        y = 1;
    }

    draw();
}