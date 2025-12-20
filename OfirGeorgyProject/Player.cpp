#include "Player.h"
#include "io_utils.h"
#include "Level.h"
#include "Color.h"
#include <iostream>
#include <cctype>   // For tolower function

using namespace std;

void Player::init(int startX, int startY, char sym, Color c, char kU, char kD, char kL, char kR, char kS, char kE) {
    x = startX;
    y = startY;
    symbol = sym;
    color = c;
    dir_x = 0;
    dir_y = 0;
    inventory = 0;

    
    //Using tolower function to always stay on lower case keys
    keyUp = tolower(kU);
    keyDown = tolower(kD);
    keyLeft = tolower(kL);
    keyRight = tolower(kR);
    keyStay = tolower(kS);
    keyDispose = tolower(kE);
}

void Player::setDirection(char key) {
    key = tolower(key);

    if (key == keyDispose) return;

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

void Player::erase(Level& level) {
    gotoxy(x, y);
    char cell = level.getCharAt(x, y);

    if (cell == ' ') cout << ' ';
    else cout << cell;
}

void Player::draw() {
    setTextColor(color);
    gotoxy(x, y);
    cout << symbol;
    setTextColor(WHITE);
}

char Player::move(Level& level) { //players' movments with wrap-around
                                  //HEIGHT and WIDTH are from Level.h 
    if (dir_x == 0 && dir_y == 0) return ' ';

    int next_x = x + dir_x;
    int next_y = y + dir_y;

    if (next_x < 0)
        next_x = WIDTH - 1;
    else if (next_x >= WIDTH)
        next_x = 0;

    if (next_y < 0)
        next_y = HEIGHT - 1;
    else if (next_y >= HEIGHT)
        next_y = 0;

    char nextCell = level.getCharAt(next_x, next_y);

    if (nextCell == '/') {
        // From open switch to closed switch
        level.setCharAt(next_x, next_y, '\\');
        }
    else if (nextCell == '\\') {
        // From closed switch to open switch
        level.setCharAt(next_x, next_y, '/');
    }
    if (nextCell == '/' || nextCell == '\\') {
        erase(level);
        x = next_x;
        y = next_y;
        draw();
        return ' ';
    }

    if (nextCell == '?') return '?';
    

    // Handle Door '3' (Level Exit Gate)
    if (nextCell == '3') {
        erase(level);
        x = next_x;
        y = next_y;
        draw();
        return '3';  // Signal level transition
    }

    // Handle Door '1' (Key-Locked)
    if (nextCell == '1') {
        if (level.isDoor1Open()) {
            // Door is open, allow passage
            erase(level);
            x = next_x;
            y = next_y;
            draw();
            return '1';
        }
        else {
            // Door is locked, check if player has a key
            if (inventory == 'K') {
                // Consume key and try to unlock door
                inventory = 0;
                if (level.tryUnlockDoor1()) {
                    // Door is now fully unlocked, remove it from map
                    level.setCharAt(next_x, next_y, ' ');
                }
                // Player stays near door (doesn't move)
                dir_x = 0;
                dir_y = 0;
                return '1';
            }
            else {
                // No key, block movement
                dir_x = 0;
                dir_y = 0;
                return ' ';
            }
        }
    }

    // Handle Door '2' (Switch-Locked)
    if (nextCell == '2') {
        if (level.isDoor2Open()) {
            // Door is open, allow passage
            erase(level);
            x = next_x;
            y = next_y;
            draw();
            return '2';
        }
        else {
            // Door is locked, block movement
            dir_x = 0;
            dir_y = 0;
            return ' ';
        }
    }

    // Handle other doors (4-9) - original logic
    if (isdigit(nextCell) && nextCell != '0') {
        if (inventory == 'K') {
            inventory = 0;
            level.setCharAt(next_x, next_y, ' ');
            erase(level);
            x = next_x;
            y = next_y;
            draw();
            return nextCell;
        }
        else {
            dir_x = 0;
            dir_y = 0;
            return ' ';
        }
    }

    if (nextCell == 'W') {
        dir_x = 0;
        dir_y = 0;
    }
    else {
        if (nextCell == 'K' || nextCell == '@' || nextCell == '!') {
            if (inventory == 0) {
                inventory = nextCell;
                level.setCharAt(next_x, next_y, ' ');
            }
        }
        erase(level);
        x = next_x;
        y = next_y;
        draw();
    }
    return ' ';
}

char Player::dispose(Level& level, int& outX, int& outY) {
    if (inventory != 0) {
        if (level.getCharAt(x, y) == ' ') {

            if (inventory == '@') {
                level.setCharAt(x, y, inventory); // Place the bomb character on the map
                outX = x;
                outY = y;
                inventory = 0;
                draw();
                return '@'; // Signal that a bomb was disposed
            }

            level.setCharAt(x, y, inventory);
            inventory = 0;
            draw();
            return 0;
        }
    }
    return 0; // Nothing was disposed
}