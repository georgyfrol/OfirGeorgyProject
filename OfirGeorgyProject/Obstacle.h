#pragma once
#include <vector>
using namespace std;

struct ObstacleCell {
    int x, y;
    ObstacleCell(int x, int y) : x(x), y(y) {}
};

class Obstacle {
private:
    vector<ObstacleCell> cells;  // List of (x, y) positions
    int size;                     // Number of cells (cells.size())

public:
    Obstacle() : size(0) {}
    
    void addCell(int x, int y) {
        cells.push_back(ObstacleCell(x, y));
        size = static_cast<int>(cells.size());
    }
    
    bool contains(int x, int y) const {
        for (const ObstacleCell& cell : cells) {
            if (cell.x == x && cell.y == y) {
                return true;
            }
        }
        return false;
    }
    
    int getSize() const { return size; }
    const vector<ObstacleCell>& getCells() const { return cells; }
    vector<ObstacleCell>& getCells() { return cells; }
    
    // Check if obstacle can be pushed in a direction
    bool canPush(int dirX, int dirY, class Level& level) const;
    
    // Push obstacle in a direction
    void push(int dirX, int dirY, class Level& level);
};

