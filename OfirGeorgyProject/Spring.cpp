#include "Spring.h"
#include "Player.h"
#include "Level.h"

void Spring::updateCompression(Player& player, Level& level) {
    int playerX = player.getX();
    int playerY = player.getY();
    
    // Check if player is on this spring
    if (!contains(playerX, playerY)) {
        if (activePlayer == &player) {
            activePlayer = nullptr;
            lastPlayerPosX = -1;
            lastPlayerPosY = -1;
        }
        return;
    }
    
    // Check if spring should release (player reached wall, pressed STAY, or changed direction)
    if (compressedLength > 0) {
        int playerDirX = player.getDirX();
        int playerDirY = player.getDirY();
        bool shouldRelease = false;
        
        // Release if player reached the wall
        int wallAdjacentX = playerX + wallDirX;
        int wallAdjacentY = playerY + wallDirY;
        if (level.getCharAt(wallAdjacentX, wallAdjacentY) == 'W') {
            shouldRelease = true;
        }
        
        // Release if player pressed STAY or changed direction
        if (playerDirX == 0 && playerDirY == 0) {
            shouldRelease = true;
        }
        if (playerDirX != wallDirX || playerDirY != wallDirY) {
            shouldRelease = true;
        }
        
        if (shouldRelease) {
            release(player, level);
            return;
        }
    }
    
    // Only one player can compress a spring at a time
    if (activePlayer != nullptr && activePlayer != &player) {
        return;
    }
    
    // Activate spring only if player is moving toward the wall
    if (activePlayer == nullptr) {
        int playerDirX = player.getDirX();
        int playerDirY = player.getDirY();
        
        if (playerDirX == wallDirX && playerDirY == wallDirY) {
            activePlayer = &player;
            lastPlayerPosX = playerX;
            lastPlayerPosY = playerY;
        } else {
            return;
        }
    }
    
    // Compression only occurs when moving toward the wall
    int playerDirX = player.getDirX();
    int playerDirY = player.getDirY();
    if (playerDirX != wallDirX || playerDirY != wallDirY) {
        lastPlayerPosX = playerX;
        lastPlayerPosY = playerY;
        return;
    }
    
    // Calculate compression: count how many spring cells the player has touched
    int currentCellIndex = -1;
    for (size_t i = 0; i < cells.size(); i++) {
        if (cells[i].x == playerX && cells[i].y == playerY) {
            currentCellIndex = static_cast<int>(i);
            break;
        }
    }
    
    if (currentCellIndex >= 0) {
        int newCompressedLength = 0;
        
        // Calculate based on wall position
        if (wallDirX == 1 || wallDirY == 1) {
            newCompressedLength = currentCellIndex + 1;
        } else {
            newCompressedLength = length - currentCellIndex;
        }
        
        // Update compression and hide one cell from the wall side
        if (newCompressedLength > compressedLength) {
            compressedLength = newCompressedLength;
            isCompressed = true;
            
            int cellIndexToHide;
            if (wallDirX == 1 || wallDirY == 1) {
                cellIndexToHide = compressedLength - 1;
            } else {
                cellIndexToHide = length - compressedLength;
            }
            
            if (cellIndexToHide >= 0 && cellIndexToHide < length) {
                level.setCharAt(cells[cellIndexToHide].x, cells[cellIndexToHide].y, ' ');
            }
        }
    }
    
    lastPlayerPosX = playerX;
    lastPlayerPosY = playerY;
}

void Spring::release(Player& player, Level& level) {
    // Restore all spring cells visually
    for (const SpringCell& cell : cells) {
        level.setCharAt(cell.x, cell.y, '#');
    }
    
    // Launch player away from wall
    int launchDirX = -wallDirX;
    int launchDirY = -wallDirY;
    int speed = compressedLength;
    int duration = compressedLength * compressedLength;
    
    player.applySpringEffect(speed, duration, launchDirX, launchDirY);
    
    // Reset spring state
    compressedLength = 0;
    isCompressed = false;
    activePlayer = nullptr;
    lastPlayerPosX = -1;
    lastPlayerPosY = -1;
}

