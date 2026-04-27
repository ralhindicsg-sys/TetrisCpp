#pragma once
#include "Board.h"
#include "PlayerManager.h"

class Game {
public:
    void Run();

private:
    Board board;
    Tetromino current;
    int x, y;
    int score;
    int level;
    int speed;
    bool paused;

    void MainMenu();
    void StartGame();
    void Loop();
    void HandleInput();
    void Spawn();
    void ClearConsole();
    bool ShowGameOver();
};