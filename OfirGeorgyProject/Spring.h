#pragma once
#include <vector>

class Player;
class Level;

using namespace std;

enum Orientation {
    HORIZONTAL,
    VERTICAL
};

struct SpringCell {
    int x, y;
    SpringCell(int _x, int _y) : x(_x), y(_y) {}
};

class Spring {
    vector<SpringCell> cells;  // List of (x, y) positions
    Orientation orientation;   // HORIZONTAL or VERTICAL
    int length;                // cells.size()
    int wallDirX, wallDirY;    // Wall direction (dx, dy)
    bool isCompressed;         // Whether spring is currently compressed
    int compressedLength;      // Number of cells hidden by compression
    const Player* activePlayer; // Player currently compressing the spring (nullptr if none)
    int lastPlayerPosX, lastPlayerPosY;  // Track previous player position for step detection
    
public:
    Spring() : orientation(HORIZONTAL), length(0), wallDirX(0), wallDirY(0), 
               isCompressed(false), compressedLength(0), activePlayer(nullptr), lastPlayerPosX(-1), lastPlayerPosY(-1) {}
    
    void addCell(int x, int y) {
        cells.push_back(SpringCell(x, y));
        length = static_cast<int>(cells.size());
    }
    
    void setOrientation(Orientation orient) {
        orientation = orient;
    }
    
    void setWallDirection(int dx, int dy) {
        wallDirX = dx;
        wallDirY = dy;
    }
    
    const vector<SpringCell>& getCells() const {
        return cells;
    }
    
    Orientation getOrientation() const {
        return orientation;
    }
    
    int getLength() const {
        return length;
    }
    
    int getWallDirX() const {
        return wallDirX;
    }
    
    int getWallDirY() const {
        return wallDirY;
    }
    
    bool contains(int x, int y) const {
        for (const auto& cell : cells) {
            if (cell.x == x && cell.y == y) {
                return true;
            }
        }
        return false;
    }
    
    // Update spring compression based on player position and movement
    void updateCompression(Player& player, Level& level);
    
    // Release spring: restore cells, launch player, reset compression
    void release(Player& player, Level& level);
};

