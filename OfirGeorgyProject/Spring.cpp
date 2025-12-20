#include "Spring.h"
#include "Player.h"
#include "Level.h"

// Update spring compression based on player position and movement
void Spring::updateCompression(Player& player, Level& level) {
    int playerX = player.getX();
    int playerY = player.getY();
    
    // STEP 5: Check if player is on spring
    if (!contains(playerX, playerY)) {
        // Player not on spring - do nothing, don't modify map
        lastPlayerPosX = playerX;
        lastPlayerPosY = playerY;
        return;
    }
    
    // Player is on spring - continue
    
    // Check release conditions FIRST (before compression)
    // Release when: player is on spring AND compressedLength > 0 AND one of:
    // 1) Player reaches the wall (adjacent in wallDir direction)
    // 2) Player presses STAY
    // 3) Player attempts to change direction (current dir != wallDir)
    if (compressedLength > 0) {
        int playerDirX = player.getDirX();
        int playerDirY = player.getDirY();
        
        bool shouldRelease = false;
        
        // Condition 1: Player reaches the wall (adjacent in wallDir direction)
        int wallAdjacentX = playerX + wallDirX;
        int wallAdjacentY = playerY + wallDirY;
        if (level.getCharAt(wallAdjacentX, wallAdjacentY) == 'W') {
            shouldRelease = true;
        }
        
        // Condition 2: Player presses STAY
        if (playerDirX == 0 && playerDirY == 0) {
            shouldRelease = true;
        }
        
        // Condition 3: Player attempts to change direction (current dir != wallDir)
        if (playerDirX != wallDirX || playerDirY != wallDirY) {
            shouldRelease = true;
        }
        
        if (shouldRelease) {
            release(player, level);
            lastPlayerPosX = playerX;
            lastPlayerPosY = playerY;
            return;
        }
    }
    
    // STEP 6: Check movement direction
    int playerDirX = player.getDirX();
    int playerDirY = player.getDirY();
    
    // Compression happens ONLY if player is moving toward the wall
    if (playerDirX != wallDirX || playerDirY != wallDirY) {
        // Player not moving toward wall - do nothing
        lastPlayerPosX = playerX;
        lastPlayerPosY = playerY;
        return;
    }
    
    // STEP 7: Compress visually
    // Check if player moved one cell (step-based compression)
    bool playerMoved = (playerX != lastPlayerPosX || playerY != lastPlayerPosY);
    
    if (playerMoved && compressedLength < length) {
        
        int cellIndex;

        // Always remove the cell FURTHEST from the wall
        if (wallDirX == 1 || wallDirY == 1) {
            // Wall is on right or bottom → remove from start
            cellIndex = compressedLength;
        } else {
            // Wall is on left or top → remove from end
            cellIndex = length - 1 - compressedLength;
        }

        level.setCharAt(cells[cellIndex].x, cells[cellIndex].y, ' ');
        compressedLength++;
        isCompressed = true;
    }
    
    // Update last player position for next cycle
    lastPlayerPosX = playerX;
    lastPlayerPosY = playerY;
}

// Release spring: restore cells, launch player, reset compression
void Spring::release(Player& player, Level& level) {
    // Restore all spring cells (#) visually in ONE game cycle
    for (const auto& cell : cells) {
        level.setCharAt(cell.x, cell.y, '#');
    }
    
    // Launch direction = opposite of wallDir
    int launchDirX = -wallDirX;
    int launchDirY = -wallDirY;
    
    // Speed = compressedLength
    int speed = compressedLength;
    
    // Duration = compressedLength * compressedLength
    int duration = compressedLength * compressedLength;
    
    // Call player.applySpringEffect(...)
    player.applySpringEffect(speed, duration, launchDirX, launchDirY);
    
    // Reset compressedLength and isCompressed
    compressedLength = 0;
    isCompressed = false;
}

