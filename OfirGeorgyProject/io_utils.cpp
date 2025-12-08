#include "io_utils.h"
#include "Color.h"
//Lab functions

using namespace std;

static bool colorEnabled = true;

void setColorMode(bool isActive) {
	colorEnabled = isActive;
}

bool isColorMode() {
	return colorEnabled;
}

void setTextColor(Color colorToSet) {//if in no colors mode then white
	if (!colorEnabled) {
		colorToSet = Color::WHITE;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colorToSet);
}

void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout << flush;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void hideCursor()
{
	HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
}

void clear_screen()
{
	system("cls");
}




