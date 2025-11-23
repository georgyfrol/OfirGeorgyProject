#pragma once

#include "Color.h"
class Player {
private:
    int x, y;
    int dir_x, dir_y;
    char symbol;
    char keyUp, keyDown, keyLeft, keyRight, keyStay, keyDispose;
    Color color;

    void erase();

public:
    void init(int startX, int startY, char sym, Color c, char kU, char kD, char kL, char kR, char kS, char kE);

    void setDirection(char key);

    void move();

    void draw();

    int getX() const { return x; }
    int getY() const { return y; }
};