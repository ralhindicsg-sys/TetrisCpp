
#include "Game.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT tempRect = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &tempRect);

    COORD size = { 120, 40 };
    SetConsoleScreenBufferSize(hConsole, size);

    SMALL_RECT rect = { 0, 0, 119, 39 };
    SetConsoleWindowInfo(hConsole, TRUE, &rect);

    system("cls");

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    Game game;
    game.Run();

    return 0;
}