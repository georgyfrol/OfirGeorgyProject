#pragma once
#include "Level.h"
#include "Riddle.h"
#include <fstream>
#include <string>
#include <iostream>
#include "io_utils.h"
#include "Color.h"

class LevelData {
public:
    static bool load(int levelNum, char map[HEIGHT][WIDTH + 1], int& legendX, int& legendY) {
        // Initialize map with spaces
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                map[i][j] = ' ';
            }
            map[i][WIDTH] = '\0';
        }

        // Initialize legend position to invalid values
        legendX = -1;
        legendY = -1;

        // Determine filename based on level number
        std::string filename;
        if (levelNum == 1) {
            filename = "adv-world_01.screen";
        }
        else if (levelNum == 2) {
            filename = "adv-world_02.screen";
        }
        else if (levelNum == 3) {
            filename = "adv-world_03.screen";
        }
        else if (levelNum == 4) {
            filename = "adv-world_04.screen";
        }
        else {
            // Invalid level number
            clear_screen();
            setTextColor(Color::LIGHTRED);
            std::cout << "ERROR: Invalid level number: " << levelNum << std::endl;
            setTextColor(Color::WHITE);
            std::cout << "\nPress any key to exit...";
            char ch = _getch();
            (void)ch;
            return false;
        }

        // Try to open the file
        std::ifstream file(filename);
        
        // If file not found, try with .txt extension
        if (!file.is_open()) {
            filename += ".txt";
            file.open(filename);
        }

        // If still not found, show error and return failure
        if (!file.is_open()) {
            clear_screen();
            setTextColor(Color::LIGHTRED);
            std::cout << "ERROR: Could not open map file: " << filename << std::endl;
            std::cout << "Please ensure the file exists in the project directory." << std::endl;
            setTextColor(Color::WHITE);
            std::cout << "\nPress any key to exit...";
            char ch = _getch();
            (void)ch;
            return false;
        }

        // Read file line by line
        std::string line;
        int row = 0;
        while (std::getline(file, line) && row < HEIGHT) {
            // Copy characters from line to map, up to WIDTH
            int col = 0;
            for (size_t i = 0; i < line.length() && col < WIDTH; i++) {
                // Skip carriage return if present
                if (line[i] != '\r') {
                    // Detect 'L' character and store its position
                    if (line[i] == 'L' && legendX == -1 && legendY == -1) {
                        legendX = col;
                        legendY = row;
                        // Replace 'L' with 'W' in map (non-walkable, but we won't draw it)
                        map[row][col] = 'W';
                    }
                    else {
                        map[row][col] = line[i];
                    }
                    col++;
                }
            }
            row++;
        }

        file.close();
        return true;
    }
};
