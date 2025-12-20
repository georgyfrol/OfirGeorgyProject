#include "Obstacle.h"
#include "Level.h"

bool Obstacle::canPush(int dirX, int dirY, Level& level) const {
    // Check if all cells can move in the given direction
    for (const ObstacleCell& cell : cells) {
        int nextX = cell.x + dirX;
        int nextY = cell.y + dirY;
        
        // Apply wrap-around
        if (nextX < 0) nextX = WIDTH - 1;
        else if (nextX >= WIDTH) nextX = 0;
        if (nextY < 0) nextY = HEIGHT - 1;
        else if (nextY >= HEIGHT) nextY = 0;
        
        char nextCell = level.getCharAt(nextX, nextY);
        
        // Cannot push if blocked by wall
        if (nextCell == 'W') {
            return false;
        }
        
        // Cannot push if blocked by another obstacle (not part of this obstacle)
        if (nextCell == '*') {
            // Check if this cell is part of this obstacle
            bool isPartOfThisObstacle = false;
            for (const ObstacleCell& c : cells) {
                if (c.x == nextX && c.y == nextY) {
                    isPartOfThisObstacle = true;
                    break;
                }
            }
            if (!isPartOfThisObstacle) {
                return false;
            }
        }
    }
    return true;
}

void Obstacle::push(int dirX, int dirY, Level& level) {
    // Move all cells of the obstacle
    for (ObstacleCell& cell : cells) {
        // Clear old position
        level.setCharAt(cell.x, cell.y, ' ');
        
        // Calculate new position
        int nextX = cell.x + dirX;
        int nextY = cell.y + dirY;
        
        // Apply wrap-around
        if (nextX < 0) nextX = WIDTH - 1;
        else if (nextX >= WIDTH) nextX = 0;
        if (nextY < 0) nextY = HEIGHT - 1;
        else if (nextY >= HEIGHT) nextY = 0;
        
        // Update cell position
        cell.x = nextX;
        cell.y = nextY;
        
        // Draw obstacle at new position
        level.setCharAt(cell.x, cell.y, '*');
    }
}

