#ifndef _IO_UTILS_H_
#define _IO_UTILS_H_

#include "Color.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
using namespace std;

// All the functions except setColorMode are from lab session

extern bool isSilentMode;
void gotoxy(int x, int y);
void setTextColor(Color);
void setColorMode(bool isActive);
bool isColorMode();
void hideCursor();
void clear_screen();

#endif