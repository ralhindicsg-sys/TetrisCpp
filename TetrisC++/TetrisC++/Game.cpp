#include "Game.h"
#include "ConsoleUtils.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <windows.h>


void Game::Run()
{
    ClearConsole();
    MainMenu();
}

void Game::MainMenu()
{
    while (true)
    {
        ClearConsole();
        int startX = 40;
        int startY = 10;

        const char* logo[] = {
            "████████╗███████╗████████╗██████╗ ██╗███████╗",
            "╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝",
            "   ██║   █████╗     ██║   ██████╔╝██║███████╗",
            "   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║",
            "   ██║   ███████╗   ██║   ██║  ██║██║███████║",
            "   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝"
        };

        int colors[] = { 11, 9, 13, 12, 14, 10 };

        for (int i = 0; i < 6; i++)
        {
            SetCursor(startX, startY + i);
            SetColor(colors[i]);
            std::cout << logo[i];
        }

        int menuX = 50;
        int menuY = 17;

        SetCursor(menuX, menuY);
        SetColor(11);
        std::cout << "1. New Player";
		

        SetCursor(menuX, menuY + 1);
        SetColor(9);
        std::cout << "2. Continue Player";
		

        SetCursor(menuX, menuY + 2);
        SetColor(13);
        std::cout << "3. Scoreboard";
       

        SetCursor(menuX, menuY + 3);
        SetColor(12);
        std::cout << "4. Clear Players";
       

        SetCursor(menuX, menuY + 4);
        SetColor(14);
        std::cout << "5. Clear Scores";
        

        SetCursor(menuX, menuY + 5);
        SetColor(4);
        std::cout << "6. Exit";

        char c = _getch();

        if (c == '1')
        {
            PlayerManager::NewPlayer();
            if (!PlayerManager::CurrentPlayer.empty())
            {
                ClearConsole();
                StartGame();
            }
        }

        if (c == '2')
        {
            PlayerManager::ContinuePlayer();
            if (!PlayerManager::CurrentPlayer.empty())
            {
                ClearConsole();
                StartGame();
            }
        }

        if (c == '3')
        {
            PlayerManager::ShowScores();
            ClearConsole();
        }

        if (c == '4'){ 
            PlayerManager::ClearPlayers(); ClearConsole(); 
        }
        if (c == '5') {
            PlayerManager::ClearScores(); ClearConsole();
        }
        if (c == '6') {
            return;
        }
    }
}

void Game::StartGame()
{
    board = Board();
    score = 0;
    level = 1;
    speed = 400;
    paused = false;

    Spawn();
    Loop();
}

void Game::Spawn()
{
    current = Tetromino::Random();
    x = Board::Width / 2 - 1;
    y = 0;
}

void Game::Loop()
{
    auto last = std::chrono::steady_clock::now();

    while (true)
    {
        HandleInput();

        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();

        if (!paused && elapsed >= speed)
        {
            y++;
            if (board.Collision(current, x, y))
            {
                y--;
                board.Lock(current, x, y);

                int cleared = board.ClearLines();
                score += cleared * 100;
                level = score / 500 + 1;
                speed = max(100, 400 - level * 40);

                Spawn();
                if (board.Collision(current, x, y))
                    break;
            }
            last = now;
        }

        board.Draw(current, x, y, score, level, paused);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    PlayerManager::SaveScore(score);

    bool restart = ShowGameOver();

    if (restart)
    {
		ClearConsole();
        StartGame();
    }
    else
    {
		ClearConsole();
        MainMenu();
    }
}
void Game::HandleInput()
{
    if (!_kbhit()) return;

    char key = _getch();

    if (key == 'a') {
        x--;
        if (board.Collision(current, x, y)) x++;
    }

    if (key == 'd') {
        x++;
        if (board.Collision(current, x, y)) x--;
    }

    if (key == 's') {
        y++;
        if (board.Collision(current, x, y)) y--;
    }

    if (key == 'w') {
        current.Rotate();
        if (board.Collision(current, x, y))
            current.Rotate();
    }

    if (key == 'p') paused = !paused;

    if (key == 27) {
        PlayerManager::SaveScore(score);
        MainMenu();
    }
}
void Game::ClearConsole()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD cellCount;

    if (hConsole == INVALID_HANDLE_VALUE) return;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, coord, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, coord, &count);

    SetConsoleCursorPosition(hConsole, coord);
}
bool Game::ShowGameOver()
{
	ClearConsole();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	int centerX = 25; 
	int centerY = 10;

    const char* gameOverLogo[] = {
 " ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ ",
 "██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗",
 "██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝",
 "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗",
 "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║",
 " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝"
    };

    int colors[] = { 12, 14, 10, 11, 13, 9 };

    for (int i = 0; i < 6; i++)
    {
        SetCursor(centerX, centerY + i);
        SetColor(colors[i % 6]);
        std::cout << gameOverLogo[i];
    }
    std::string playerText = "Player: " + PlayerManager::CurrentPlayer;
    SetCursor((consoleWidth - playerText.length()) / 2, centerY + 7);
    SetColor(11);
    std::cout << playerText;

    std::string scoreText = "Score: " + std::to_string(score);
    SetCursor((consoleWidth - scoreText.length()) / 2, centerY + 8);
    SetColor(9);
    std::cout << scoreText;

    std::string levelText = "Level: " + std::to_string(level);
    SetCursor((consoleWidth - levelText.length()) / 2, centerY + 9);
    SetColor(13);
    std::cout << levelText;


    std::string restartText = "Press R to Restart";
    int restartX = (consoleWidth - restartText.length()) / 2;
    int restartY = centerY + 11;

   
    SetCursor(restartX, restartY);
    SetColor(7);
    std::cout << restartText;

    bool visible = true;
    auto last = std::chrono::steady_clock::now();

    std::string escText = "Press ESC to Menu";
    SetCursor((consoleWidth - escText.length()) / 2, centerY + 12);
    SetColor(12);
    std::cout << escText;


    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();

        if (elapsed > 600)
        {
            visible = !visible;
            last = now;

            SetCursor(restartX, restartY);

            if (visible)
            {
                SetColor(7);
                std::cout << restartText;
            }
            else
            {
                std::cout << std::string(restartText.length(), ' ');
            }
        }

        if (_kbhit())
        {
            char key = _getch();

            if (key == 'r' || key == 'R')
                return true;

            if (key == 27)
                return false;
        }
    }
}