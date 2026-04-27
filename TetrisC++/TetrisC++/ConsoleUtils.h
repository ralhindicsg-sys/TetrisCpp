#pragma once
#include <windows.h>

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

inline void SetCursor(int x, int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

inline void SetColor(int color)
{
    SetConsoleTextAttribute(hConsole, color);
}