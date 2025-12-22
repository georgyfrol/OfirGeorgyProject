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
    bool stayPressedThisCycle;
    
    // Spring effect fields
    int speed; // Default = 1, set to compressedLength on release
    int remainingSpringCycles; // Default = 0, set to compressedLength * compressedLength on release
    int springDirX, springDirY; // Spring launch direction (only valid when spring is active)
    
    // Track previous direction to detect direction changes
    int prevDirX, prevDirY;

public:
    // STAY input flag for spring release detection (public for Spring access)
   // bool stayPressedThisCycle;
    void erase(Level& level);
    void init(int startX, int startY, char sym, Color c, char kU, char kD, char kL, char kR, char kS, char kE);

    // returns the character of the item disposed, and sets the disposal location.
    char dispose(Level& level, int& outX, int& outY);

    void setDirection(char key);

    char move(Level& level, Player* otherPlayer = nullptr);

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
    
    // Spring effect methods
    void setSpeed(int s) { speed = s; }
    int getSpeed() const { return speed; }
    void setRemainingSpringCycles(int cycles) { remainingSpringCycles = cycles; }
    int getRemainingSpringCycles() const { return remainingSpringCycles; }
    void setSpringDirection(int dirX, int dirY) { springDirX = dirX; springDirY = dirY; }
    void getSpringDirection(int& dirX, int& dirY) const { dirX = springDirX; dirY = springDirY; }
    bool isUnderSpringEffect() const { return remainingSpringCycles > 0; }
    void applySpringEffect(int newSpeed, int newCycles, int newDirX, int newDirY);
    
    // Helper methods for spring release detection
    bool hasDirectionChanged() const;
    bool pressedStay() const;
    
    // STAY input flag methods
    void setStayPressed(bool pressed) { stayPressedThisCycle = pressed; }
    void resetStayPressed() { stayPressedThisCycle = false; }
    
    // Force calculation for obstacle pushing
    int getForce() const;  // Returns 1 normally, or speed if under spring effect
};