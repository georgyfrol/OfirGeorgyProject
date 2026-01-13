#include "io_utils.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>

using namespace std;

bool isSilentMode = false;
bool isColorActive = true;
void setColorMode(bool isActive) {
    isColorActive = isActive;
}

bool isColorMode() {
    return isColorActive;
}

void gotoxy(int x, int y)
{
    if (isSilentMode) return;
    HANDLE hConsoleOutput;
    COORD dwCursorPosition;
    cout.flush();
    dwCursorPosition.X = x;
    dwCursorPosition.Y = y;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}


void setTextColor(Color color)
{
    if (isSilentMode) return;

    if (!isColorActive) {
        color = Color::WHITE;
    }

    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleOutput, (WORD)color);
}

void hideCursor()
{
    if (isSilentMode) return;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void clear_screen()
{
    if (isSilentMode) return;
    system("cls");
}