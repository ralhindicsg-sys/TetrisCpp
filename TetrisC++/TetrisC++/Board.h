#pragma once
#include "Tetromino.h"

class Board {
public:
    static const int Width = 10;
    static const int Height = 25;

    int grid[Height][Width] = {};
    int colors[Height][Width] = {};

    bool Collision(Tetromino& t, int px, int py);
    void Lock(Tetromino& t, int px, int py);
    int ClearLines();
    void Draw(Tetromino& t, int px, int py, int score, int level, bool paused);
};