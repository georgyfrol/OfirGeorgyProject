#pragma once
#include "Level.h"
#include <cmath>

// Define the constant for the bomb's fuse time in game cycles
const int BOMB_FUSE_TIME = 5;

class Bomb {
private:
    int x, y; // Bomb position
    int counter; // Countdown timer (starts at BOMB_FUSE_TIME)
    bool active; // Is the bomb currently active/ticking

    // Helper function to calculate squared distance
    int distSq(int x1, int y1, int x2, int y2) {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }

public:
    // Constructor: initializes the bomb at a specific location
    Bomb(int startX, int startY) : x(startX), y(startY), counter(BOMB_FUSE_TIME), active(true) {}

    // Advances the countdown by one game cycle. Returns true if it explodes.
    bool advance(Level& level);

    // Executes the explosion logic.
    void explode(Level& level);

    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }
};