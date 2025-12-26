#include "Game.h"

int main() {
    // Initializes the Game object and calls the main loop
    Game game;
    int result = game.run();
    return result;  // Return 0 on success, 1 on fatal error
}