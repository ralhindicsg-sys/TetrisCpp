#include "Board.h"
#include "ConsoleUtils.h"
#include "PlayerManager.h"
#include <iostream>

bool Board::Collision(Tetromino& t, int px, int py)
{
    for (int y = 0; y < t.H(); y++)
        for (int x = 0; x < t.W(); x++)
            if (t.Shape[y][x] == 1)
            {
                int fx = px + x;
                int fy = py + y;
                if (fx < 0 || fx >= Width || fy >= Height) return true;
                if (fy >= 0 && grid[fy][fx] == 1) return true;
            }
    return false;
}

void Board::Lock(Tetromino& t, int px, int py)
{
    for (int y = 0; y < t.H(); y++)
        for (int x = 0; x < t.W(); x++)
            if (t.Shape[y][x] == 1)
            {
                grid[py + y][px + x] = 1;
                colors[py + y][px + x] = t.Color;
            }
}

int Board::ClearLines()
{
    int cleared = 0;
    for (int y = Height - 1; y >= 0; y--)
    {
        bool full = true;
        for (int x = 0; x < Width; x++)
            if (grid[y][x] == 0) full = false;

        if (full)
        {
            for (int yy = y; yy > 0; yy--)
                for (int x = 0; x < Width; x++)
                {
                    grid[yy][x] = grid[yy - 1][x];
                    colors[yy][x] = colors[yy - 1][x];
                }
            cleared++;
            y++;
        }
    }
    return cleared;
}

void Board::Draw(Tetromino& t, int px, int py, int score, int level, bool paused)
{
    int offsetX = 40;
    int offsetY = 5;

    SetCursor(offsetX, offsetY - 2);
    SetColor(11);
    std::cout << "Player: " << PlayerManager::CurrentPlayer;

    SetCursor(offsetX + 25, offsetY - 2);
    SetColor(9);
    std::cout << "Score: " << score;

    SetCursor(offsetX + 45, offsetY - 2);
    SetColor(13);
    std::cout << "Level: " << level;

    for (int y = 0; y < Height; y++)
    {
        SetCursor(offsetX, offsetY + y);

        for (int x = 0; x < Width; x++)
        {
            bool piece = false;

            for (int ty = 0; ty < t.H(); ty++)
                for (int tx = 0; tx < t.W(); tx++)
                    if (t.Shape[ty][tx] == 1 && py + ty == y && px + tx == x)
                    {
                        SetColor(t.Color);
                        std::cout << "[#]";
                        piece = true;
                    }

            if (!piece)
            {
                if (grid[y][x] == 1)
                {
                    SetColor(colors[y][x]);
                    std::cout << "[#]";
                }
                else
                {
                    SetColor(8);
                    std::cout << " . ";
                }
            }
        }
    }

    SetColor(7);
}