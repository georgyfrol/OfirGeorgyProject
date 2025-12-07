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
    void erase(Level& level);

public:
    void init(int startX, int startY, char sym, Color c, char kU, char kD, char kL, char kR, char kS, char kE);

    // Signature change: returns the character of the item disposed, and sets the disposal location.
    char dispose(Level& level, int& outX, int& outY);

    void setDirection(char key);

    char move(Level& level);

    void draw();

    void dispose(Level& level);

    int getX() const { return x; }
    int getY() const { return y; }

    char getInventory() const { return inventory; }


};