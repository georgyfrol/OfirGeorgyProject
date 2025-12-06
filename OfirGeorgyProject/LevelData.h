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
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[1]  = "W                                                                              W";
            layout[2]  = "W                                                                              W";
            layout[3]  = "W                                                                              W";
            layout[4]  = "W        WWWWWWWWWWW                                                           W";
            layout[5]  = "W        W         W                                                           W";
            layout[6]  = "W        W  K   K  W                           @                               W";
            layout[7]  = "W        W         W                                                           W";
            layout[8]  = "W        WWWWW1WWWWW                                                           W";
            layout[9]  = "W                                                                              W";
            layout[10] = "W                                                                              W";
            layout[11] = "W                                                                              W";
            layout[12] = "W                              K        WWWWWWWWWWWW                           W";
            layout[13] = "W                                       W   W      W                           W";
            layout[14] = "W                                       W   W  K   W                           W";
            layout[15] = "W                                       1   1      W                           W";
            layout[16] = "W                                       W   W  @   W                           W";
            layout[17] = "W                                       W   W      W                           W";
            layout[18] = "W                                       WWWWWWWWWWWW                           W";
            layout[19] = "W                                                                              W";
            layout[20] = "W                                                                              W";
            layout[21] = "W                                                                              W";
            layout[22] = "W                                                                              W";
            layout[23] = "W                                                                              W";
            layout[24] = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            break;

        case 2:
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[1]  = "W                                                                              W";
            layout[2]  = "W   K                                                                          W";
            layout[3]  = "W                                                                              W";
            layout[4]  = "W        WWWWWWWW                                                              W";
            layout[5]  = "W        W      W                                                              W";
            layout[6]  = "W        W      W                                                              W";
            layout[7]  = "W        W      W                                                              W";
            layout[8]  = "W        WWWWWWWW                                                              W";
            layout[9]  = "W                                                                              W";
            layout[10] = "W                                                                              W";
            layout[11] = "W                                          #                                   W";
            layout[12] = "W                                          #                                   W";
            layout[13] = "W                                          #             !                     W";
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

        case 3:
            layout[0]  = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
            layout[10] = "W                                  THE END                                     W";
            layout[12] = "W                             CONGRATULATIONS                                  W";
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