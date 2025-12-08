#pragma once

#include "Color.h"
#include "Level.h"

class Player {
private:
    int x, y;
    int dir_x, dir_y;
    char symbol;
    char keyUp, keyDown, keyLeft, keyRight, keyStay, keyDispose;
    Color color;
    char inventory = 0;

public:
    void erase(Level& level);
    void init(int startX, int startY, char sym, Color c, char kU, char kD, char kL, char kR, char kS, char kE);

    // returns the character of the item disposed, and sets the disposal location.
    char dispose(Level& level, int& outX, int& outY);

    void setDirection(char key);

    char move(Level& level);

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
    void stop() { dir_x = 0; dir_y = 0; }

    void draw();

    int getX() const { return x; }
    int getY() const { return y; }
    int getDirX() const { return dir_x; }
    int getDirY() const { return dir_y; }

    char getInventory() const { return inventory; }
    void setInventory(char item) { inventory = item; }


};