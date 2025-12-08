#pragma once
#include "Level.h"

class LevelData {
public:
    static void load(int levelNum, char map[HEIGHT][WIDTH + 1]) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                map[i][j] = ' ';
            }
            map[i][WIDTH] = '\0';
        }

        const char* layout[HEIGHT] = { nullptr };

        switch (levelNum) {
        
        case 1:
            // Level 1: Bomb & Keys Cooperative Challenge
            // Key 1: In walled chamber (top), accessible by bombing wall
            // Key 2: At end of convoluted path (right side)
            // Door '1' blocks path to Door '3', requires 2 Keys
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[1]  = "W  @  W                                                                          W";
            layout[2]  = "W  W  W                                                                          W";
            layout[3]  = "W  K  W                                                                          W";
            layout[4]  = "W  WWWW                                                                         W";
            layout[5]  = "W  W  W                                                                          W";
            layout[6]  = "W                                                                                W";
            layout[7]  = "W        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[8]  = "W        W                                                                       W";
            layout[9]  = "W        W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[10] = "W        W  W                                                                   W";
            layout[11] = "W        W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[12] = "W        W  W  W                                                               W";
            layout[13] = "W        W  W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[14] = "W        W  W  W  W                                                           W";
            layout[15] = "W        W  W  W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[16] = "W        W  W  W  W  W                                                       W";
            layout[17] = "W        W  W  W  W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[18] = "W        W  W  W  W  W  W                                                   W";
            layout[19] = "W        W  W  W  W  W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[20] = "W        W  W  W  W  W  W  W                                               W";
            layout[21] = "W        W  W  W  W  W  W  W  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[22] = "W        W  W  W  W  W  W  W  W                                           W";
            layout[23] = "W        W  W  W  W  W  W  W  W  K                   1               3   W";
            layout[24] = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            break;

        case 2:
            // Level 2: Cooperative Switch Puzzle
            // Door '2' blocks path to Door '3'
            // Switch 1 (S): Accessible only to Player 1 (left side, isolated area)
            // Switch 2 (S): Accessible only to Player 2 (right side, isolated area)
            // Both switches must be held simultaneously to open Door '2'
            // Key (K) placed in center - requires coordination to reach
            // Door '3' at exit (bottom right)
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[1]  = "W  S  W                                                                  W  S  W";
            layout[2]  = "W  W  W                                                                  W  W  W";
            layout[3]  = "W  W  W                                                                  W  W  W";
            layout[4]  = "W  W  W                                                                  W  W  W";
            layout[5]  = "W  W  W                                                                  W  W  W";
            layout[6]  = "W  W  W                                                                  W  W  W";
            layout[7]  = "W  W  W                                                                  W  W  W";
            layout[8]  = "W  W  W                                                                  W  W  W";
            layout[9]  = "W  W  W                                                                  W  W  W";
            layout[10] = "W  W  W                                                                  W  W  W";
            layout[11] = "W  W  W                                                                  W  W  W";
            layout[12] = "W  W  W                                                                  W  W  W";
            layout[13] = "W  W  W                                                                  W  W  W";
            layout[14] = "W  W  W                                                                  W  W  W";
            layout[15] = "W  W  W                                                                  W  W  W";
            layout[16] = "W  W  W                                                                  W  W  W";
            layout[17] = "W  W  W                                                                  W  W  W";
            layout[18] = "W  W  W                                                                  W  W  W";
            layout[19] = "W  W  W                                                                  W  W  W";
            layout[20] = "W  W  W                                                                  W  W  W";
            layout[21] = "W  W  W                                                                  W  W  W";
            layout[22] = "W  W  W                              K                   2               W  W  W";
            layout[23] = "W  W  W                                                                    3   W";
            layout[24] = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            break;

        case 3:
            // Level 3: End Screen - all boundaries must be 'W'
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[1]  = "W                                                                              W";
            layout[2]  = "W                                                                              W";
            layout[3]  = "W                                                                              W";
            layout[4]  = "W                                                                              W";
            layout[5]  = "W                                                                              W";
            layout[6]  = "W                                                                              W";
            layout[7]  = "W                                                                              W";
            layout[8]  = "W                                                                              W";
            layout[9]  = "W                                                                              W";
            layout[10] = "W                                  THE END                                     W";
            layout[11] = "W                                                                              W";
            layout[12] = "W                             CONGRATULATIONS                                  W";
            layout[13] = "W                                                                              W";
            layout[14] = "W                                                                              W";
            layout[15] = "W                                                                              W";
            layout[16] = "W                                                                              W";
            layout[17] = "W                                                                              W";
            layout[18] = "W                                                                              W";
            layout[19] = "W                                                                              W";
            layout[20] = "W                                                                              W";
            layout[21] = "W                                                                              W";
            layout[22] = "W                                                                              W";
            layout[23] = "W                                                                              W";
            layout[24] = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            break;
        }

        for (int i = 0; i < HEIGHT; i++) {
            if (layout[i] != nullptr) {
                for (int j = 0; j < WIDTH; j++) {
                    char c = layout[i][j];
                    if (c == '\0') break;
                    map[i][j] = c;
                }
            }
        }
    }
};
