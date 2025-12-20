#include "Player.h"
#include "io_utils.h"
#include "Level.h"
#include "Color.h"
#include "Obstacle.h"
#include <iostream>
#include <cctype>   // For tolower function
#include <cmath>    // For abs function

using namespace std;

void Player::init(int startX, int startY, char sym, Color c, char kU, char kD, char kL, char kR, char kS, char kE) {
    x = startX;
    y = startY;
    symbol = sym;
    color = c;
    dir_x = 0;
    dir_y = 0;
    inventory = 0;
    speed = 1;
    remainingSpringCycles = 0;
    springDirX = 0;
    springDirY = 0;
    prevDirX = 0;
    prevDirY = 0;

    
    //Using tolower function to always stay on lower case keys
    keyUp = tolower(kU);
    keyDown = tolower(kD);
    keyLeft = tolower(kL);
    keyRight = tolower(kR);
    keyStay = tolower(kS);
    keyDispose = tolower(kE);
}

void Player::applySpringEffect(int newSpeed, int newCycles, int newDirX, int newDirY) {
    speed = newSpeed;
    remainingSpringCycles = newCycles;
    springDirX = newDirX;
    springDirY = newDirY;
    // Set direction to spring direction
    dir_x = springDirX;
    dir_y = springDirY;
}

int Player::getForce() const {
    // Normal force is 1, but if under spring effect, force equals speed
    if (remainingSpringCycles > 0) {
        return speed;
    }
    return 1;
}

void Player::setDirection(char key) {
    key = tolower(key);

    if (key == keyDispose) return;

    // Store previous direction before changing
    prevDirX = dir_x;
    prevDirY = dir_y;

    // During spring effect, allow only sideways movement (perpendicular to spring direction)
    if (remainingSpringCycles > 0) {
        // Block STAY and backward movement
        if (key == keyStay) {
            return;
        }
        
        // Determine the input direction
        int newDirX = 0, newDirY = 0;
        if (key == keyUp) {
            newDirX = 0; newDirY = -1;
        }
        else if (key == keyDown) {
            newDirX = 0; newDirY = 1;
        }
        else if (key == keyLeft) {
            newDirX = -1; newDirY = 0;
        }
        else if (key == keyRight) {
            newDirX = 1; newDirY = 0;
        }
        
        // Block backward movement against spring direction
        if ((springDirX != 0 && newDirX == -springDirX) || 
            (springDirY != 0 && newDirY == -springDirY)) {
            return;
        }
        
        // Allow sideways movement (perpendicular to spring direction)
        if (springDirX != 0) {
            // Spring is horizontal, allow vertical sideways
            if (newDirY != 0) {
                dir_x = newDirX;
                dir_y = newDirY;
            }
        } else if (springDirY != 0) {
            // Spring is vertical, allow horizontal sideways
            if (newDirX != 0) {
                dir_x = newDirX;
                dir_y = newDirY;
            }
        }
        return;
    }

    // Normal movement (no spring effect)
    if (key == keyUp) {
        dir_x = 0; dir_y = -1;
    }
    else if (key == keyDown) {
        dir_x = 0; dir_y = 1;
    }
    else if (key == keyLeft) {
        dir_x = -1; dir_y = 0;
    }
    else if (key == keyRight) {
        dir_x = 1; dir_y = 0;
    }
    else if (key == keyStay) {
        dir_x = 0; dir_y = 0;
    }
}

bool Player::hasDirectionChanged() const {
    return (dir_x != prevDirX || dir_y != prevDirY);
}

bool Player::pressedStay() const {
    // Check if player is stopped (dir is 0,0) and was moving before
    // This indicates STAY key was pressed
    return (dir_x == 0 && dir_y == 0 && (prevDirX != 0 || prevDirY != 0));
}

void Player::erase(Level& level) {
    gotoxy(x, y);
    char cell = level.getCharAt(x, y);

    if (cell == ' ') cout << ' ';
    else cout << cell;
}

void Player::draw() {
    setTextColor(color);
    gotoxy(x, y);
    cout << symbol;
    setTextColor(WHITE);
}

char Player::move(Level& level, Player* otherPlayer) { //players' movments with wrap-around
                                  //HEIGHT and WIDTH are from Level.h 
    
    // Handle spring acceleration effect
    if (remainingSpringCycles > 0) {
        char collisionResult = ' ';
        
        // STEP-BY-STEP FORWARD MOVEMENT: repeat 'speed' times, 1 cell at a time
        for (int step = 0; step < speed; step++) {
            int next_x = x + springDirX;
            int next_y = y + springDirY;
            
            // Apply wrap-around
            if (next_x < 0) next_x = WIDTH - 1;
            else if (next_x >= WIDTH) next_x = 0;
            if (next_y < 0) next_y = HEIGHT - 1;
            else if (next_y >= HEIGHT) next_y = 0;
            
            char nextCell = level.getCharAt(next_x, next_y);
            
            // Check for wall collision - if wall, stop spring immediately and stay in place
            if (nextCell == 'W') {
                remainingSpringCycles = 0;
                speed = 1;
                springDirX = 0;
                springDirY = 0;
                dir_x = 0;
                dir_y = 0;
                return ' ';
            }
            
            // Check for obstacle
            if (nextCell == '*') {
                Obstacle* obstacle = level.getObstacleAt(next_x, next_y);
                if (obstacle != nullptr) {
                    // Calculate total force
                    int totalForce = getForce();
                    
                    // Check if other player is adjacent and moving in same direction
                    if (otherPlayer != nullptr) {
                        int otherX = otherPlayer->getX();
                        int otherY = otherPlayer->getY();
                        int otherDirX = otherPlayer->getDirX();
                        int otherDirY = otherPlayer->getDirY();
                        
                        // Check if other player is adjacent (directly adjacent, not diagonal)
                        bool isAdjacent = (abs(otherX - x) + abs(otherY - y) == 1);
                        
                        // Check if other player is moving in same direction
                        bool sameDirection = (otherDirX == springDirX && otherDirY == springDirY);
                        
                        if (isAdjacent && sameDirection) {
                            totalForce += otherPlayer->getForce();
                        }
                    }
                    
                    // Check if force is sufficient to push obstacle
                    if (totalForce >= obstacle->getSize()) {
                        // Push obstacle in the direction of movement
                        if (obstacle->canPush(springDirX, springDirY, level)) {
                            obstacle->push(springDirX, springDirY, level);
                        } else {
                            // Obstacle cannot be pushed (blocked by wall or another obstacle)
                            remainingSpringCycles = 0;
                            speed = 1;
                            springDirX = 0;
                            springDirY = 0;
                            dir_x = 0;
                            dir_y = 0;
                            return ' ';
                        }
                    } else {
                        // Insufficient force, block movement
                        remainingSpringCycles = 0;
                        speed = 1;
                        springDirX = 0;
                        springDirY = 0;
                        dir_x = 0;
                        dir_y = 0;
                        return ' ';
                    }
                }
            }
            
            // Check for collision with other player
            if (otherPlayer != nullptr && next_x == otherPlayer->getX() && next_y == otherPlayer->getY()) {
                // Transfer spring effect to other player
                otherPlayer->applySpringEffect(speed, remainingSpringCycles, springDirX, springDirY);
            }
            
            // Handle switches
            if (nextCell == '/') {
                level.setCharAt(next_x, next_y, '\\');
            }
            else if (nextCell == '\\') {
                level.setCharAt(next_x, next_y, '/');
            }
            
            // Handle special cells that block movement
            if (nextCell == '1') {
                // Door '1' - check if open
                if (!level.isDoor1Open()) {
                    // Door is locked, stop spring immediately
                    remainingSpringCycles = 0;
                    speed = 1;
                    springDirX = 0;
                    springDirY = 0;
                    dir_x = 0;
                    dir_y = 0;
                    return '1'; // Return door collision code
                }
            }
            
            // Move player one cell forward
            erase(level);
            x = next_x;
            y = next_y;
            draw();
            
            // Check for collision returns (doors, items, etc.)
            if (nextCell == '?') {
                collisionResult = '?';
                break; // Stop forward movement on item collision
            }
            if (nextCell == '3') {
                collisionResult = '3';
                break; // Stop forward movement on door collision
            }
            if (nextCell == '1' && level.isDoor1Open()) {
                collisionResult = '1';
                break; // Stop forward movement on door collision
            }
            if (nextCell == '2') {
                collisionResult = '2';
                break; // Stop forward movement on door collision
            }
            if (isdigit(nextCell) && nextCell != '0') {
                collisionResult = nextCell;
                break; // Stop forward movement on digit collision
            }
        }
        
        // OPTIONAL SIDEWAYS MOVEMENT: at most 1 cell per cycle, perpendicular only
        int sidewaysX = 0;
        int sidewaysY = 0;
        
        if (springDirX != 0) {
            // Spring is horizontal, allow vertical sideways movement
            if (dir_y != 0) {
                sidewaysY = dir_y;  // Normal speed (1)
            }
        } else if (springDirY != 0) {
            // Spring is vertical, allow horizontal sideways movement
            if (dir_x != 0) {
                sidewaysX = dir_x;  // Normal speed (1)
            }
        }
        
        // Prevent backward movement against spring direction
        if ((springDirX != 0 && dir_x == -springDirX) || 
            (springDirY != 0 && dir_y == -springDirY)) {
            sidewaysX = 0;
            sidewaysY = 0;
        }
        
        // Apply sideways movement (one extra single-cell step)
        if (sidewaysX != 0 || sidewaysY != 0) {
            int next_x = x + sidewaysX;
            int next_y = y + sidewaysY;
            
            // Apply wrap-around
            if (next_x < 0) next_x = WIDTH - 1;
            else if (next_x >= WIDTH) next_x = 0;
            if (next_y < 0) next_y = HEIGHT - 1;
            else if (next_y >= HEIGHT) next_y = 0;
            
            char nextCell = level.getCharAt(next_x, next_y);
            
            // Check for wall collision
            if (nextCell == 'W') {
                // Wall blocks sideways movement, but don't stop spring
                // Player stays at current position
            }
            // Check for obstacle
            else if (nextCell == '*') {
                Obstacle* obstacle = level.getObstacleAt(next_x, next_y);
                if (obstacle != nullptr) {
                    // Calculate total force (for sideways, use normal force = 1)
                    int totalForce = 1;  // Sideways movement is always at normal speed
                    
                    // Check if other player is adjacent and moving in same sideways direction
                    if (otherPlayer != nullptr) {
                        int otherX = otherPlayer->getX();
                        int otherY = otherPlayer->getY();
                        int otherDirX = otherPlayer->getDirX();
                        int otherDirY = otherPlayer->getDirY();
                        
                        // Check if other player is adjacent (directly adjacent, not diagonal)
                        bool isAdjacent = (abs(otherX - x) + abs(otherY - y) == 1);
                        
                        // Check if other player is moving in same sideways direction
                        bool sameDirection = (otherDirX == sidewaysX && otherDirY == sidewaysY);
                        
                        if (isAdjacent && sameDirection) {
                            totalForce += 1;  // Other player's sideways force is also 1
                        }
                    }
                    
                    // Check if force is sufficient to push obstacle
                    if (totalForce >= obstacle->getSize()) {
                        // Push obstacle in the sideways direction
                        if (obstacle->canPush(sidewaysX, sidewaysY, level)) {
                            obstacle->push(sidewaysX, sidewaysY, level);
                            // Move player after pushing obstacle
                            erase(level);
                            x = next_x;
                            y = next_y;
                            draw();
                        } else {
                            // Obstacle cannot be pushed, block sideways movement
                            // Player stays at current position
                        }
                    } else {
                        // Insufficient force, block sideways movement
                        // Player stays at current position
                    }
                }
            }
            // Check for collision with other player
            else if (otherPlayer != nullptr && next_x == otherPlayer->getX() && next_y == otherPlayer->getY()) {
                // Transfer spring effect to other player
                otherPlayer->applySpringEffect(speed, remainingSpringCycles, springDirX, springDirY);
                // Move player
                erase(level);
                x = next_x;
                y = next_y;
                draw();
            }
            // Check for locked door
            else if (nextCell == '1' && !level.isDoor1Open()) {
                // Door blocks sideways movement, but don't stop spring
                // Player stays at current position
            }
            // Otherwise, allow sideways movement
            else {
                // Handle switches
                if (nextCell == '/') {
                    level.setCharAt(next_x, next_y, '\\');
                }
                else if (nextCell == '\\') {
                    level.setCharAt(next_x, next_y, '/');
                }
                
                erase(level);
                x = next_x;
                y = next_y;
                draw();
                
                // Update collision result if needed
                if (nextCell == '?') collisionResult = '?';
                else if (nextCell == '3') collisionResult = '3';
                else if (nextCell == '1' && level.isDoor1Open()) collisionResult = '1';
                else if (nextCell == '2') collisionResult = '2';
                else if (isdigit(nextCell) && nextCell != '0') collisionResult = nextCell;
            }
        }
        
        // Decrement cycles AFTER all movement
        remainingSpringCycles--;
        
        // If spring ends now → reset and STAY in place afterwards
        if (remainingSpringCycles == 0) {
            speed = 1;
            springDirX = 0;
            springDirY = 0;
            dir_x = 0;
            dir_y = 0;
        }
        
        return collisionResult;
    }
    
    // Normal movement (no spring effect)
    if (dir_x == 0 && dir_y == 0) return ' ';

    int next_x = x + dir_x;
    int next_y = y + dir_y;

    if (next_x < 0)
        next_x = WIDTH - 1;
    else if (next_x >= WIDTH)
        next_x = 0;

    if (next_y < 0)
        next_y = HEIGHT - 1;
    else if (next_y >= HEIGHT)
        next_y = 0;

    char nextCell = level.getCharAt(next_x, next_y);

    if (nextCell == '/') {
        // From open switch to closed switch
        level.setCharAt(next_x, next_y, '\\');
        }
    else if (nextCell == '\\') {
        // From closed switch to open switch
        level.setCharAt(next_x, next_y, '/');
    }
    if (nextCell == '/' || nextCell == '\\') {
        erase(level);
        x = next_x;
        y = next_y;
        draw();
        return ' ';
    }

    // Check for obstacle
    if (nextCell == '*') {
        Obstacle* obstacle = level.getObstacleAt(next_x, next_y);
        if (obstacle != nullptr) {
            // Calculate total force
            int totalForce = getForce();
            
            // Check if other player is adjacent and moving in same direction
            if (otherPlayer != nullptr) {
                int otherX = otherPlayer->getX();
                int otherY = otherPlayer->getY();
                int otherDirX = otherPlayer->getDirX();
                int otherDirY = otherPlayer->getDirY();
                
                // Check if other player is adjacent (directly adjacent, not diagonal)
                bool isAdjacent = (abs(otherX - x) + abs(otherY - y) == 1);
                
                // Check if other player is moving in same direction
                bool sameDirection = (otherDirX == dir_x && otherDirY == dir_y);
                
                if (isAdjacent && sameDirection) {
                    totalForce += otherPlayer->getForce();
                }
            }
            
            // Check if force is sufficient to push obstacle
            if (totalForce >= obstacle->getSize()) {
                // Push obstacle in the direction of movement
                if (obstacle->canPush(dir_x, dir_y, level)) {
                    obstacle->push(dir_x, dir_y, level);
                    // Move player after pushing obstacle
                    erase(level);
                    x = next_x;
                    y = next_y;
                    draw();
                    return ' ';
                } else {
                    // Obstacle cannot be pushed (blocked by wall or another obstacle)
                    dir_x = 0;
                    dir_y = 0;
                    return ' ';
                }
            } else {
                // Insufficient force, block movement
                dir_x = 0;
                dir_y = 0;
                return ' ';
            }
        }
    }

    if (nextCell == '?') return '?';
    

    // Handle Door '3' (Level Exit Gate)
    if (nextCell == '3') {
        erase(level);
        x = next_x;
        y = next_y;
        draw();
        return '3';  // Signal level transition
    }

    // Handle Door '1' (Key-Locked)
    if (nextCell == '1') {
        if (level.isDoor1Open()) {
            // Door is open, allow passage
            erase(level);
            x = next_x;
            y = next_y;
            draw();
            return '1';
        }
        else {
            // Door is locked, check if player has a key
            if (inventory == 'K') {
                // Consume key and try to unlock door
                inventory = 0;
                if (level.tryUnlockDoor1()) {
                    // Door is now fully unlocked, remove it from map
                    level.setCharAt(next_x, next_y, ' ');
                }
                // Player stays near door (doesn't move)
                dir_x = 0;
                dir_y = 0;
                return '1';
            }
            else {
                // No key, block movement
                dir_x = 0;
                dir_y = 0;
                return ' ';
            }
        }
    }

    // Handle Door '2' (Switch-Locked)
    if (nextCell == '2') {
        if (level.isDoor2Open()) {
            // Door is open, allow passage
            erase(level);
            x = next_x;
            y = next_y;
            draw();
            return '2';
        }
        else {
            // Door is locked, block movement
            dir_x = 0;
            dir_y = 0;
            return ' ';
        }
    }

    // Handle other doors (4-9) - original logic
    if (isdigit(nextCell) && nextCell != '0') {
        if (inventory == 'K') {
            inventory = 0;
            level.setCharAt(next_x, next_y, ' ');
            erase(level);
            x = next_x;
            y = next_y;
            draw();
            return nextCell;
        }
        else {
            dir_x = 0;
            dir_y = 0;
            return ' ';
        }
    }

    if (nextCell == 'W') {
        dir_x = 0;
        dir_y = 0;
    }
    else {
        if (nextCell == 'K' || nextCell == '@' || nextCell == '!') {
            if (inventory == 0) {
                inventory = nextCell;
                level.setCharAt(next_x, next_y, ' ');
            }
        }
        erase(level);
        x = next_x;
        y = next_y;
        draw();
    }
    return ' ';
}

char Player::dispose(Level& level, int& outX, int& outY) {
    if (inventory != 0) {
        if (level.getCharAt(x, y) == ' ') {

            if (inventory == '@') {
                level.setCharAt(x, y, inventory); // Place the bomb character on the map
                outX = x;
                outY = y;
                inventory = 0;
                draw();
                return '@'; // Signal that a bomb was disposed
            }

            level.setCharAt(x, y, inventory);
            inventory = 0;
            draw();
            return 0;
        }
    }
    return 0; // Nothing was disposed
}