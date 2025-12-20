#include "Spring.h"
#include "Player.h"
#include "Level.h"

// Update spring compression based on player position and movement
void Spring::updateCompression(Player& player, Level& level) {
    int playerX = player.getX();
    int playerY = player.getY();
    
    // STEP 5: Check if player is on spring
    if (!contains(playerX, playerY)) {
        // Player not on spring - clear activePlayer if this was the active player
        if (activePlayer == &player) {
            activePlayer = nullptr;
            lastPlayerPosX = -1;
            lastPlayerPosY = -1;
        }
        return;
    }
    
    // Player is on spring - continue
    
    // Check release conditions FIRST (before any direction-based early returns)
    // Release when: compressedLength > 0 AND player is on spring AND one of:
    // 1) Player reaches the wall (adjacent in wallDir direction)
    // 2) Player presses STAY (direction is 0,0)
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
            return;
        }
    }
    
    // If activePlayer is set and it's not this player, ignore this player completely
    if (activePlayer != nullptr && activePlayer != &player) {
        return;
    }
    
    // If no active player, check if this player should become active
    if (activePlayer == nullptr) {
        int playerDirX = player.getDirX();
        int playerDirY = player.getDirY();
        
        // Set activePlayer only if player is moving toward the wall
        if (playerDirX == wallDirX && playerDirY == wallDirY) {
            activePlayer = &player;
            lastPlayerPosX = playerX;
            lastPlayerPosY = playerY;
        } else {
            // Player is on spring but not moving toward wall - don't activate
            return;
        }
    }
    
    // Now we know activePlayer == &player, continue with compression logic
    
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
    // compressedLength = number of spring cells ('#') the player has TOUCHED (been on)
    // Find which spring cell the player is currently on
    int currentCellIndex = -1;
    for (size_t i = 0; i < cells.size(); i++) {
        if (cells[i].x == playerX && cells[i].y == playerY) {
            currentCellIndex = static_cast<int>(i);
            break;
        }
    }
    
    if (currentCellIndex >= 0) {
        // Calculate compressedLength based on how many cells player has TOUCHED
        int newCompressedLength = 0;
        
        if (wallDirX == 1 || wallDirY == 1) {
            // Wall is on right or bottom → cells[0] is closest to wall
            // compressedLength = number of cells touched from wall side to current position
            newCompressedLength = currentCellIndex + 1;
        } else {
            // Wall is on left or top → cells[length-1] is closest to wall
            // compressedLength = number of cells touched from wall side to current position
            newCompressedLength = length - currentCellIndex;
        }
        
        // Update compressedLength only if player moved forward (toward wall)
        if (newCompressedLength > compressedLength) {
            compressedLength = newCompressedLength;
            isCompressed = true;
            
            // Hide exactly one cell (the one player just passed) starting from wall side
            int cellIndexToHide;
            if (wallDirX == 1 || wallDirY == 1) {
                // Wall on right/bottom → hide from start
                cellIndexToHide = compressedLength - 1;
            } else {
                // Wall on left/top → hide from end
                cellIndexToHide = length - compressedLength;
            }
            
            if (cellIndexToHide >= 0 && cellIndexToHide < length) {
                level.setCharAt(cells[cellIndexToHide].x, cells[cellIndexToHide].y, ' ');
            }
        }
    }
    
    // Update last player position for next cycle
    lastPlayerPosX = playerX;
    lastPlayerPosY = playerY;
}

// Release spring: restore cells, launch player, reset compression
void Spring::release(Player& player, Level& level) {
    // Restore ALL spring cells unconditionally from Spring::cells
    // Do NOT check map state - cells vector is the source of truth
    for (const SpringCell& cell : cells) {
        level.setCharAt(cell.x, cell.y, '#');
    }
    
    // Launch direction = opposite of wallDir
    int launchDirX = -wallDirX;
    int launchDirY = -wallDirY;
    
    // Speed = compressedLength (N blocks per cycle)
    int speed = compressedLength;
    
    // Duration = compressedLength * compressedLength (N^2 game cycles)
    int duration = compressedLength * compressedLength;
    
    // Call player.applySpringEffect(...)
    player.applySpringEffect(speed, duration, launchDirX, launchDirY);
    
    // Reset compressedLength and isCompressed
    compressedLength = 0;
    isCompressed = false;
    
    // Reset activePlayer and last position
    activePlayer = nullptr;
    lastPlayerPosX = -1;
    lastPlayerPosY = -1;
}

