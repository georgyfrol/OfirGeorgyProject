#pragma once
#include "Level.h"
#include "Riddle.h"


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
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[1]  = "W        ?                          W                           ?              W";
            layout[2]  = "W  WWWWWWWWWWWW  WWWWWWWWWWWWWWWWW  W  WWWWWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWW  W";
            layout[3]  = "W  W          W  W               W  W  W                  W  W              W  W";
            layout[4]  = "W  W  WWWWWW  W  W  WWWWWWWWWWW  W  W  W  WWWWWWWWWWWWWW  W  W  WWWWWW      W  W";
            layout[5]  = "W  W  W       W  W  W            W     W  W            W  W  W      \\W         W";  // double backslash prints as a single one
            layout[6]  = "W  W  W  WWWWWW  W  W  WWWWWWWWWWWWWWWWW  W  WWWWWWWW  W  W  WWWWWWWWW      W  W";
            layout[7]  = "W     W          W            W           W            W  W         \\W      W  W";
            layout[8]  = "WWWWWWWW       WWW  WWWWWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWW  WWWWWWWWWWWW      WWWW";
            layout[9]  = "W####               W                                             **       ####W";
            layout[10] = "WWWWWWWW            W  WWWWWWWWWWWWWWWW2WWWWWWWWWWWWWWW  WWWWWWWWWWWWW     WWWWW";
            layout[11] = "W                   W  W                         W  K W                        W";
            layout[12] = "W  WWWWWWWWWWWWWWWWWW  W    @                    W    W  WWWWWWWWWWWWWWWWWWWW  W";
            layout[13] = "W  W                   W                         WWWWWW                     W  W";
            layout[14] = "W  W  WWWWWWWWWWWWWWWWWW                              WWWWWWWWWWWWWWWW      W  W";
            layout[15] = "W  W                   WWWWWW                         W                     W  W";
            layout[16] = "W  WWWWWWWWWWWWWWWWWW  W    W                     @   W  WWWWWWWWWWWWWWWWWWWW  W";
            layout[17] = "W                   W  W K  W                         W                        W";
            layout[18] = "WWWWWWWWWWWWWWWWWWWWW  WWWWWWWWWWWWWWWW1WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[19] = "W                   W                                                          W";
            layout[20] = "W  WWWWWWWWWWWWWWWWWWWWWWWW  WWWWWWWWWW?WWWWWWWWW  WWWWWWWWWWWWWWWWWWWWWWWWWW  W";
            layout[21] = "W                            W  W            W  W                              W";
            layout[22] = "W  WWWWWWWWWWWWWWWWWWWWWWWW  W  WWWWWWW?WWWWWW  W  WWWWWWWWWWWWWWWWWWWWWWWWWW  W";
            layout[23] = "W                            W                  W                              W";
            layout[24] = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW3WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            break;

        case 2:
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[1]  = "W  \\  W                                                                  W  \\  W";  
            layout[2]  = "W  W  W                                                                  W  W  W";
            layout[3]  = "W  W  W                                                                  W  W  W";
            layout[4]  = "W  W  W                                     @                            W  W  W";
            layout[5]  = "W  W  W                                                                  W  W  W";
            layout[6]  = "W  W  W                                                                  W  W  W";
            layout[7]  = "W  W  W                                                                  W  W  W";
            layout[8]  = "W  W  W                                                                  W  W  W";
            layout[9]  = "W  W  W                              2222222                             W  W  W";
            layout[10] = "W  W  W                 @            2     2                             W  W  W";
            layout[11] = "W  W  W                              2  3  2             @               W  W  W";
            layout[12] = "W  W  W                              2     2                             W  W  W";
            layout[13] = "W  W  W                              2222222                             W  W  W";
            layout[14] = "W  W  W                                                                  W  W  W";
            layout[15] = "W  W  W                                                                  W  W  W";
            layout[16] = "W  W  W                                                                  W  W  W";
            layout[17] = "W  W  W                                                                  W  W  W";
            layout[18] = "W  W  W                                                                  W  W  W";
            layout[19] = "W  W  W                                                                  W  W  W";
            layout[20] = "W  W  W                                                                  W  W  W";
            layout[21] = "W  W  W                                                                  W  W  W";
            layout[22] = "W  W  W                              @                                   W  W  W";
            layout[23] = "W  W  2                                                                  2  W  W";
            layout[24] = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            break;

        case 3:
            // Level 3: End Screen 
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
    static vector<Riddle> getRiddles(int levelNum) {
        vector<Riddle> list;

        if (levelNum == 1) {
            list.push_back({ "What has roots as nobody sees,\n                    Is taller than trees,\n                    Up, up it goes,\n                    And yet never grows?", "mountain" });
            list.push_back({ "Alive without breath,\n                    As cold as death;\n                    Never thirsty, ever drinking,\n                    All in mail never clinking.", "fish" });
            list.push_back({ "What has keys but can't open locks?", "piano" });
            list.push_back({ "I speak without a mouth. What am I?", "echo" });
        }

        return list;
    }
};
