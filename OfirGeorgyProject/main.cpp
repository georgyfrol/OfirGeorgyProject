#include "Game.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    // Initializes the Game object and calls the main loop
    Game game;
    game.run(argc, argv);
    return 0; // Return 0 on success, 1 on fatal error
}