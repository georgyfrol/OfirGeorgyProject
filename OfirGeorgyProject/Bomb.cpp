#include "Bomb.h"
#include "io_utils.h" // For gotoxy, setTextColor
#include <iostream>
#include <cstdlib> // For abs (used implicitly in the explosion check)

using namespace std;

bool Bomb::advance(Level& level) {
    if (!active) return false;

    // Display the countdown number on the bomb's position
    gotoxy(x, y);
    setTextColor(Color::RED);
    cout << counter;
    setTextColor(Color::WHITE);

    counter--;

    if (counter <= 0) {
        // Bomb exploded
        explode(level);
        active = false;
        return true;
    }
    return false;
}

//Gemini function: We wrote gemini to help us with the explsion
void Bomb::explode(Level& level) { 
    // Clear the bomb character itself from the screen
    level.setCharAt(x, y, ' ');

    // Explosion radius is 3, including diagonals. Distance squared <= 9.
    const int RADIUS_SQ = 9; // 3^2

    // Loop over a 7x7 box around the bomb (from -3 to +3)
    for (int dy = -3; dy <= 3; ++dy) {
        for (int dx = -3; dx <= 3; ++dx) {
            int current_x = x + dx;
            int current_y = y + dy;

            // Check if the current cell is within the screen bounds
            if (current_x >= 0 && current_x < WIDTH && current_y >= 0 && current_y < HEIGHT) {

                // Check if the current cell is within the radius (Distance^2 <= 9)
                if (distSq(x, y, current_x, current_y) <= RADIUS_SQ) {

                    char cell = level.getCharAt(current_x, current_y);

                    // Special rule for Walls: only adjacent walls are diminished.
                    // Adjacent means dx and dy are 0 or 1. If both are > 1, it's not adjacent.
                    // This implementation assumes: a Wall is shielded if the distance > 1.
                    if (cell == 'W' && (abs(dx) > 1 || abs(dy) > 1)) {
                        continue; // Skip destruction
                    }

                    // Diminishes any other object (Obstacles, Keys, Torches, etc.)
                    if (cell != ' ') {
                        level.setCharAt(current_x, current_y, ' ');
                    }
                }
            }
        }
    }
}